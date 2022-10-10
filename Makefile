NAME	= philo
CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread -g #-fsanitize=thread -g

SRC_PATH = sources/
OBJ_PATH = objects/

SRC		=	main.c \
			philosopher.c \
			reaper.c \
			simu.c \
			time_and_print.c \
			init_table_philo.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean