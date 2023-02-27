#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int StrLen(const char *str)
{
    int i=0;
    while(str[i])
    {
        i++;
    }
    return i;
}

void Error2(const char *s1,const char *s2)
{
    write(STDERR_FILENO,s1,StrLen(s1));
    write(STDERR_FILENO,s2,StrLen(s2));
    write(STDERR_FILENO,"\0",1);
}
void Error(const char *s1)
{
    write(STDERR_FILENO,s1,StrLen(s1));
    write(STDERR_FILENO,"\0",1);
}


int main()
{
    pid_t pid = fork();
    if (pid==-1)
    {
        Error("error : fatal");
    }
    if (pid==0)//child
    {
         char *args[]={"/bin/ls","-l",NULL};
        if (execv(args[0], args)==-1)
        {
            Error2("Fail execute ",args[0]);
            exit(EXIT_FAILURE);    
        }
        exit(EXIT_SUCCESS);
    } else
    {
        int status=0;
        waitpid(pid,&status,0);
    }
}

