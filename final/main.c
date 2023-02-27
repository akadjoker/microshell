#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

#define TYPE_END	3
#define TYPE_PIPE	4
#define TYPE_BREAK	5



int ft_strlen(const char *str)
{
    int i=0;
    while(str[i])
        i++;
    return i;
}

void	ft_error2(char *s1, char *s2)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_error(char *s1)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	write(STDERR_FILENO, "\n", 1);
}


void execute_line(char **args, char **env, int is_pipe) 
{
    int fd[2];
    if (is_pipe) 
        pipe(fd);

    pid_t pid = fork();
    if (pid == 0)  // child process
    { 
        if (is_pipe)
        {
            close(fd[0]);               // close the read end of the pipe
            dup2(fd[1], STDOUT_FILENO); // redirect stdout to the write end of the pipe
            close(fd[1]);               // close the write end of the pipe
        }
         if (execve(args[0], &args[0], env) == -1)
         {
             ft_error2("error: cannot execute ",args[0]);
             exit(EXIT_FAILURE);
         }
         exit(EXIT_SUCCESS);// yes no?? 
    } else 
    { 
        int status;
        if (is_pipe) 
        {
            close(fd[1]); // close the write end of the pipe
            dup2(fd[0], STDIN_FILENO); // redirect stdin to the read end of the pipe
            close(fd[0]); // close the read end of the pipe
        }

        waitpid(pid, &status, 0);
    }
}

int size_argv(char **argv)
{
    int i = 0;
    while (argv[i] && strcmp(argv[i], "|") != 0 && strcmp(argv[i], ";") != 0)
        i++;
    return (i);
}

int check_end(char *av)
{
    if (!av)
        return (TYPE_END);
    if (strcmp(av, "|") == 0)
        return (TYPE_PIPE);
    if (strcmp(av, ";") == 0)
        return (TYPE_BREAK);
    return (0);
}

void handle_cd(char** args) 
{
    if (args[1] == NULL) 
    {
        ft_error("error: cd: bad arguments");
        return;
    }

    if (chdir(args[1]) == -1) 
    {
        ft_error2("error: cd: cannot change directory to ", args[1]);
    }
}

int parse_comands( char **av, char **env)
{
  
    int count = size_argv(av);
	int index= 0;
	int type = check_end(av[count]);
	char **argv = (char **)malloc(sizeof(char *) * (count + 1));
	while (index < count)
	{
		argv[index] = strdup(av[index]);
        index++;
	}
    argv[index] = NULL;///the last arg mas beeeee nullo
    if (count)
    {
        if (strcmp(argv[0], "cd") == 0)
        {
            handle_cd(argv);
        } else
        {
            if (type==TYPE_PIPE)
            {
                execute_line(argv,env, true);
            } else
            {
                execute_line(argv, env,false);
            }
        }
    }
    index = 0;
    while (index < count)
	{
		free(argv[index]);
        index++;
	}
    free(argv);
    return count;
}

int	main(int argc, char **argv, char **env)
{


	if (argc < 2)
		return (0);

	int i = 1;
	while (argv[i])
    {
            if (strcmp(argv[i], ";") == 0)
            {
                i++;
                continue ;
            }
    	    i += parse_comands(&argv[i],env);
			if (argv[i])
				i++;
			else
				break;
    }
	
	return 0;
}

