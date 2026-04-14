NAME = minishell

# PATHS

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

INC = ./includes

SRC_DIR = ./sources

OBJ_DIR = ./obj

# Source files

SOURCES = main.c lexer/lexer.c lexer/utils_lexer.c lexer/utils_lex_aux.c prompt.c parser/parser.c \
		parser/utils.c parser/utils_parser_aux.c expander/expander.c expander/utils_exp.c signals.c freeing.c \
		expander/expander_tokens.c expander/expander_2.c expander/expander_3.c expander/env_extra.c\
		execution/execution.c execution/execution_cmd.c execution/heredoc.c execution/heredoc_utils.c execution/heredoc2.c \
		execution/redirections.c execution/path_helper_funcs.c execution/envp.c execution/ex_helper.c execution/execution_aux.c \
		builtins/ft_echo.c builtins/ft_env.c builtins/ft_exit.c builtins/ft_export.c builtins/ft_export_utils.c builtins/ft_pwd.c \
		builtins/ft_unset.c builtins/builtins.c builtins/export_print.c builtins/ft_cd.c 

SRCS = $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compiler flags
CC = cc -g 

CFLAGS = -Wall -Wextra -Werror -I $(INC)

# Rules
all: $(NAME)

$(LIBFT):
	@echo "Libft is compiling..."
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null
	@echo "Libft is compiled!"
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME) > /dev/null
	@echo "minishell is compiled!"

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null
	@echo "Objects cleaned!"

fclean: clean
	@echo "Cleaning project..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null
	@echo "All cleaned!"

re: fclean all

v:				$(NAME)
				clear && valgrind --show-leak-kinds=all --leak-check=full --track-fds=all --track-origins=yes --suppressions=readline.supp ./minishell


.PHONY: all clean fclean re