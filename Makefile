# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 15:36:34 by eala-lah          #+#    #+#              #
#    Updated: 2024/12/05 14:49:45 by eala-lah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Project Name and Libraries ---
# Specifies the project name, library directories, and required libraries.
NAME_SERVER  = server
NAME_CLIENT  = client
INCS         = -I ./inc/ -I ./libft/inc/
LIBFT_DIR    = libft/
LIBFT        = $(LIBFT_DIR)/libft.a

# --- Source and Object Directories ---
# Defines directories for source code and object files.
SRC_DIR      = src/
SRC_SERVER   = \
    server.c \
    utils.c

SRC_CLIENT   = \
    client.c \
    utils.c

OBJ_DIR      = obj/
OBJS_SERVER  = $(addprefix $(OBJ_DIR), $(SRC_SERVER:.c=.o))
OBJS_CLIENT  = $(addprefix $(OBJ_DIR), $(SRC_CLIENT:.c=.o))

# --- Compiler and Flags ---
# Specifies the compiler, compilation flags, and necessary library flags.
CC           = gcc
CFLAGS       = -Wall -Wextra -Werror $(INCS) -fPIC
GIT_FLAGS    = git clone --depth 1

# --- Default Target ---
# Builds all dependencies and final executable
all: $(LIBFT) $(OBJ_DIR) $(NAME_SERVER) $(NAME_CLIENT)

# --- Directory Setup ---
# Creates the object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR) 2> /dev/null || { echo "Failed to create object directory." >&2; exit 1; }

# --- libft Library Setup ---
# Installs and builds the libft library, cloning the repository if not already present.
$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		$(GIT_FLAGS) https://github.com/erkkaervice/libft.git $(LIBFT_DIR) > /dev/null 2>&1 || { echo "Failed to clone libft repository." >&2; exit 1; }; \
	fi
	@make -C $(LIBFT_DIR) CFLAGS="-Wall -Wextra -Werror -fPIC -I ./inc/" > /dev/null 2>&1 || { echo "Failed to build libft library." >&2; exit 1; }
	@rm -f $(LIBFT_DIR)/README.md > /dev/null 2>&1 || { echo "Failed to remove README.md." >&2; exit 1; }

# --- Compilation ---
# Compiles source files into object files.
$(OBJ_DIR)%.o: $(SRC_DIR)%.c inc/minitalk.h
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@ 2> /dev/stderr || { echo "Failed to compile $<." >&2; exit 1; }

# --- Linking ---
# Links object files and creates the final executable.
$(NAME_SERVER): $(OBJS_SERVER)
	@$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(NAME_SERVER) -L $(LIBFT_DIR) -lft 2> /dev/stderr || { echo "Failed to create executable $(NAME_SERVER)." >&2; exit 1; }

$(NAME_CLIENT): $(OBJS_CLIENT)
	@$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(NAME_CLIENT) -L $(LIBFT_DIR) -lft 2> /dev/stderr || { echo "Failed to create executable $(NAME_CLIENT)." >&2; exit 1; }

# --- Bonus Target ---
# Bonus target to build everything (equivalent to the 'all' target)
bonus: all

# --- Cleaning ---
# Removes object files and cleans the libraries
clean:
	@rm -rf $(OBJ_DIR) 2> /dev/null || { echo "Failed to clean object files." >&2; exit 1; }
	@make -C $(LIBFT_DIR) clean > /dev/null 2>&1 || { if [ -d "$(LIBFT_DIR)" ]; then echo "Failed to clean libft." >&2; exit 1; fi; }

# --- Full Clean ---
# Full clean target to remove everything including the executable and libraries.
fclean: clean
	@rm -f $(LIBFT) $(NAME_SERVER) $(NAME_CLIENT) 2> /dev/stderr || { echo "Failed to remove generated files." >&2; exit 1; }
	@rm -rf $(LIBFT_DIR) 2> /dev/stderr || { if [ -d "$(LIBFT_DIR)" ]; then echo "Failed to remove libft directory." >&2; exit 1; fi; }

# --- Rebuild ---
# Rebuilds everything by cleaning and running all again
re: fclean all

# --- Phony Targets ---
# Declares all targets as phony to avoid conflicts with files of the same name
.PHONY: all clean fclean re bonus
