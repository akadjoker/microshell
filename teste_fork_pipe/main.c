#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

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
    write(STDERR_FILENO,"\n",1);
}
void Error(const char *s1)
{
    write(STDERR_FILENO,s1,StrLen(s1));
    write(STDERR_FILENO,"\n",1);
}


void execute(char **args, bool isPipe)
{
    int fd[2]={0,0};
    if (isPipe)
    {
        if (pipe(fd)==-1)
        {
             Error("error : fatal");
             exit(EXIT_FAILURE);
        }
    }

    //o fork tem k ser sempre depois do fork
    pid_t pid = fork();
    if (pid==-1)
    {
        Error("error : fatal");
        exit(EXIT_FAILURE); 
    }

    if (pid==0) 
    {
            if (isPipe)
            {
                close(fd[0]);
                dup2(fd[1],STDOUT_FILENO);
                close(fd[1]);
            }
            if (execv(args[0],args)==-1)
            {
                Error2("error execute",args[0]);
                exit(EXIT_FAILURE);
            }
            Error2("Execute",args[0]);
            
    } else
    {

        if(isPipe)
        {
            close(fd[1]);
            dup2(fd[0],STDIN_FILENO);
            close(fd[0]);
        }
        int status;
        waitpid(pid,&status,0);
    }
}

int main()
{
   
   {
         char *args[]={"/bin/ls","-l",NULL};
         execute(args,true);
   }
   {
         char *args[]={"/bin/cat","-e",NULL};
          execute(args,false);
   }
      Error("End");
}

