# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 15:36:34 by eala-lah          #+#    #+#              #
#    Updated: 2024/10/07 12:02:35 by eala-lah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SERVER  = server
NAME_CLIENT  = client

INCS        = -I ./libft/inc/ -I ./inc/
LIBFT_DIR   = libft/
LIBFT       = $(LIBFT_DIR)/libft.a

SRC_DIR     = src/
SRC         = \
    server.c \
    client.c \
    utils.c

SRCS        = $(addprefix $(SRC_DIR), $(SRC))
OBJ_DIR     = obj/
OBJS        = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -fPIC
GIT_FLAGS   = git clone --depth 1

all: $(LIBFT) $(OBJ_DIR) $(NAME_SERVER) $(NAME_CLIENT)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "Created object directory."

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		$(GIT_FLAGS) https://github.com/erkkaervice/libft.git $(LIBFT_DIR) || exit 1; \
	fi
	@make -C $(LIBFT_DIR) CFLAGS="-Wall -Wextra -Werror -fPIC -I ./inc/" 2> /dev/stderr > /dev/null
	@echo "Libft library built."

$(OBJ_DIR)%.o: $(SRC_DIR)%.c inc/minitalk.h
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@ 2> /dev/stderr > /dev/null
	@echo "Compiled $< into $@."

$(NAME_SERVER): $(OBJ_DIR)server.o $(OBJ_DIR)utils.o
	@$(CC) $(CFLAGS) $(OBJ_DIR)server.o $(OBJ_DIR)utils.o -o $(NAME_SERVER) -L $(LIBFT_DIR) -lft 2> /dev/stderr > /dev/null
	@echo "Executable $(NAME_SERVER) created."

$(NAME_CLIENT): $(OBJ_DIR)client.o $(OBJ_DIR)utils.o
	@$(CC) $(CFLAGS) $(OBJ_DIR)client.o $(OBJ_DIR)utils.o -o $(NAME_CLIENT) -L $(LIBFT_DIR) -lft 2> /dev/stderr > /dev/null
	@echo "Executable $(NAME_CLIENT) created."

bonus: all

clean:
	@rm -rf $(OBJ_DIR) 2> /dev/stderr > /dev/null
	@make -C $(LIBFT_DIR) clean 2> /dev/stderr > /dev/null
	@echo "Cleaned up $(OBJ_DIR) and library."

fclean: clean
	@rm -f $(LIBFT) $(NAME_SERVER) $(NAME_CLIENT) 2> /dev/stderr > /dev/null
	@rm -rf $(LIBFT_DIR) 2> /dev/stderr > /dev/null
	@echo "Fully cleaned up all generated files."

re: fclean all

.PHONY: all clean fclean re bonus
