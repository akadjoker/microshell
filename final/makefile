NAME = main
FLAGS =-Wall -Wextra -Werror -g -fsanitize=address

all:
	@gcc $(FLAGS) -c $(NAME).c 
	@gcc $(FLAGS) $(NAME).o -o $(NAME)
	@./$(NAME) /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell

run:
	#@./$(NAME) /bin/ls
	#@./$(NAME) /bin/ls microshell.c
	#@./$(NAME) ";" ";" /bin/echo OK
	#@./$(NAME) ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" blah "|" /bin/echo OK ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";" ";"





clean:
	rm -f *.o $(NAME)