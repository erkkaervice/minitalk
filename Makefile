# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/20 15:47:32 by eala-lah          #+#    #+#              #
#    Updated: 2024/10/04 13:49:44 by eala-lah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SERVER = server
NAME_CLIENT = client
BONUS_SERVER = server_bonus
BONUS_CLIENT = client_bonus

INCS        = -I ./inc/
LIBFT_DIR   = libft/
LIBFT       = $(LIBFT_DIR)/libft.a

SRC_DIR     = src/
SRC         = \
	server.c \
	client.c \
	utils.c \
	server_bonus.c \
	client_bonus.c \
	utils_bonus.c

OBJ_DIR     = obj/
OBJS        = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

all: $(OBJ_DIR) $(LIBFT) $(NAME_SERVER) $(NAME_CLIENT)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	if [ ! -d "$(LIBFT_DIR)" ]; then \
		git clone git@github.com:erkkaervice/libft.git $(LIBFT_DIR); \
	fi
	make -C $(LIBFT_DIR) CFLAGS="-Wall -Wextra -Werror -fPIC"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c inc/minitalk.h inc/minitalk_bonus.h
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(NAME_SERVER): $(OBJ_DIR)server.o $(OBJ_DIR)utils.o
	$(CC) $(CFLAGS) $(OBJ_DIR)server.o $(OBJ_DIR)utils.o -o $(NAME_SERVER) -L $(LIBFT_DIR) -lft

$(NAME_CLIENT): $(OBJ_DIR)client.o $(OBJ_DIR)utils.o
	$(CC) $(CFLAGS) $(OBJ_DIR)client.o $(OBJ_DIR)utils.o -o $(NAME_CLIENT) -L $(LIBFT_DIR) -lft

bonus: $(OBJ_DIR) $(LIBFT) $(BONUS_SERVER) $(BONUS_CLIENT)

$(BONUS_SERVER): $(OBJ_DIR)server_bonus.o $(OBJ_DIR)utils_bonus.o
	$(CC) $(CFLAGS) $(OBJ_DIR)server_bonus.o $(OBJ_DIR)utils_bonus.o -o $(BONUS_SERVER) -L $(LIBFT_DIR) -lft

$(BONUS_CLIENT): $(OBJ_DIR)client_bonus.o $(OBJ_DIR)utils_bonus.o
	$(CC) $(CFLAGS) $(OBJ_DIR)client_bonus.o $(OBJ_DIR)utils_bonus.o -o $(BONUS_CLIENT) -L $(LIBFT_DIR) -lft

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(LIBFT)
	rm -rf $(LIBFT_DIR)
	rm -f $(NAME_SERVER) $(NAME_CLIENT)
	rm -f $(BONUS_SERVER) $(BONUS_CLIENT)

re: fclean all

.PHONY: all clean fclean re bonus
