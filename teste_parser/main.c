#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define PIPE      1
#define SEPARATOR 2
#define END       3

int count_args(char **av)
{
    int i=0;
    // se nao temos fim da linha e o char nao é pipe ou ; continuamos
    while(av[i] && strcmp(av[i],";")!=0 && strcmp(av[i],"|")!=0)
    {
        i++;
    }
    return i;
}
int last_cmd(char *str)
{
    if (!str)
        return END;
    
    if (strcmp(str,"|")==0)
        return PIPE;
    
    if (strcmp(str,";")==0)
        return SEPARATOR;

    return END;
}

int parser( char **av)
{
    int count = count_args(av);
    int type  = last_cmd(av[count]);
    bool isPipe = (type==PIPE);
    printf(" size %d %d \n",count,type);
    int index =0;
    while(index <count)
    {
        printf(" %d %s \n",index, av[index]);
        index++;
    }
    return count;
}

int main(int ac, char **av)
{
    if (ac<2)
        return 1;

    int i=1;
    while(av[i])
    {
        if (strcmp(av[i],";")==0)
        {
            i++;
            continue;
        }
        i += parser(&av[i]);
        if (av[i])
            i++;
        else
            break;

    }
    return 0;
}

