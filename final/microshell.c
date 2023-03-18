#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE  5
#define END   4
#define BREAK 3

/*
STDERR "error: cd: bad arguments" followed by a '\n'
error: cannot execute executable_that_failed" in STDERR followed by a '\n' w
STDERR "error: cd: cannot change directory to path_to_change" followed by a '\n' with
"error: fatal" in STDERR
*/
int StrLen(const char *s)
{
    int i=0;
    while(s[i])
    {
        i++;
    }
    return i;
}

void Error (const char* s)
{
    write(STDERR_FILENO,s,StrLen(s));
    write(STDERR_FILENO,"\n",1);
}

void Error2 (const char* s,const char *s2)
{
    write(STDERR_FILENO,s,StrLen(s));
    write(STDERR_FILENO,s2,StrLen(s2));
    write(STDERR_FILENO,"\n",1);
}


void ExecCD(char **args, int count)
{
    if (count != 2)
    {
        Error("error: cd: bad arguments");   
        return;
    }
    if (args[1]==NULL)
    {
        Error("error: cd: bad arguments");
        return;
    }
    if (chdir(args[1])==1)
    {
        Error2("error: cd: cannot change directory to",args[1]);
    }
}

int CountArgs(char **av)
{
    int i=0;
    while(av[i] && strcmp(av[i],"|")!=0 && strcmp(av[i],";")!=0)
    {
        i++;
    }
    return i;
}

char * StrDup( char* s)
{
    int size = StrLen(s);
    char * str = malloc((size + 1) * sizeof(char));
    str[size]= '\0';
    int i=0;
    while (i<size)
    {
        str[i]=s[i];
        i++;
    }
    return str;
}

int ArgType(char* s)
{
    if (!s)
        return END;
    if (strcmp(s,"|")==0)
        return PIPE;
    if (strcmp(s,";")==0)
        return BREAK;
    return END;
}

void Execute( char *args[], char *env[], bool isPipe)
{
    int fd[2];

    if (isPipe)
    {
       pipe(fd);
    }

    pid_t pid = fork();
    if (pid==0)
    {
        if (isPipe)
        {
            close(fd[0]);
            dup2(fd[1],STDOUT_FILENO);
            close(fd[1]);
        }
        if (execve(args[0], &args[0], env)==-1)
        {
             Error2("error: cannot execute ",args[0]);
             exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);

    } else
    {   
        if (isPipe)
        {
            close(fd[1]);
            dup2(fd[0],STDIN_FILENO);
            close(fd[0]);
        }
        int s;
        waitpid(pid,&s,0);
    }

}

int Parse(char **av,  char **env)
{
    int count= CountArgs(av);
    int type = ArgType(av[count]);//qual é o ultimo
    int index =0;
    bool isPipe = (type==PIPE);

   // printf(" %d %d \n",count,type);
    char **args = (char**) malloc((count +1) * sizeof(char*));
    
    while (index < count)
    {   
        args[index] = strdup(av[index]);
        index++;
    }
    args[index]=NULL;
    if (count)
    {
        if (strcmp(args[0],"cd")==0)
        {
            ExecCD(args, count);
        } else
        {
          Execute(args,env,isPipe);
        }
    }

    index =0;
    while (index < count)
    {   
        free(args[index]);
        index++;
    }
    free(args);

    return count;
}


int main(int ac,  char **av, char **env)
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
        i += Parse(&av[i],env);
        if (av[i])
        {
            i++;
        } else
        {
            break;
        }

    }

    return 0;
}