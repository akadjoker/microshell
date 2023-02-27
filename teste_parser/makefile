NAME = main
FLAGS = -g -Werror -Wall -Wextra -fsanitize=address

all:
	gcc $(NAME).c $(FLAGS) -o $(NAME)
	@./$(NAME) /bin/ls -e "|" /bin/cat -e
	@rm -f *.o $(NAME) 
	


