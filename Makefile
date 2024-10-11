# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 15:36:34 by eala-lah          #+#    #+#              #
#    Updated: 2024/10/11 14:05:03 by eala-lah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SERVER  = server
NAME_CLIENT  = client

INCS        = -I ./inc/ -I ./libft/inc/
LIBFT_DIR   = libft/
LIBFT       = $(LIBFT_DIR)/libft.a

SRC_DIR     = src/
SRC_SERVER  = \
    server.c \
    utils.c

SRC_CLIENT  = \
    client.c \
    utils.c

OBJ_DIR     = obj/
OBJS_SERVER = $(addprefix $(OBJ_DIR), $(SRC_SERVER:.c=.o))
OBJS_CLIENT = $(addprefix $(OBJ_DIR), $(SRC_CLIENT:.c=.o))

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror $(INCS) -fPIC
GIT_FLAGS   = git clone --depth 1

all: $(LIBFT) $(OBJ_DIR) $(NAME_SERVER) $(NAME_CLIENT)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR) 2> /dev/null || { echo "Failed to create object directory."; exit 1; }

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		$(GIT_FLAGS) https://github.com/erkkaervice/libft.git $(LIBFT_DIR) > /dev/null 2>&1 || { echo "Failed to clone libft repository."; exit 1; }; \
	fi
	@make -C $(LIBFT_DIR) CFLAGS="-Wall -Wextra -Werror -fPIC -I ./inc/" 2> /dev/null > /dev/null || { echo "Failed to build libft library."; exit 1; }

$(OBJ_DIR)%.o: $(SRC_DIR)%.c inc/minitalk.h
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@ 2> /dev/null || { echo "Failed to compile $<."; exit 1; }

$(NAME_SERVER): $(OBJS_SERVER)
	@$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(NAME_SERVER) -L $(LIBFT_DIR) -lft 2> /dev/null || { echo "Failed to create executable $(NAME_SERVER)."; exit 1; }

$(NAME_CLIENT): $(OBJS_CLIENT)
	@$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(NAME_CLIENT) -L $(LIBFT_DIR) -lft 2> /dev/null || { echo "Failed to create executable $(NAME_CLIENT)."; exit 1; }

bonus: all

clean:
	@rm -rf $(OBJ_DIR) 2> /dev/null || { echo "Failed to clean object files." >&2; }
	@make -C $(LIBFT_DIR) clean > /dev/null 2>&1 || { echo "Failed to clean libft."; }

fclean: clean
	@rm -f $(LIBFT) $(NAME_SERVER) $(NAME_CLIENT) 2> /dev/null || { echo "Failed to remove generated files."; }
	@rm -rf $(LIBFT_DIR) 2> /dev/null || { echo "Failed to remove libft directory."; }

re: fclean all

.PHONY: all clean fclean re bonus
