NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I include -D_GNU_SOURCE -I/opt/homebrew/opt/readline/include

SRCS = main/main.c \
	parser/parser.c \
	parser/parsing.c \
	parser/lexer.c \
	parser/expander.c \
	parser/quote_handler.c \
	parser/parsing_norm.c \
	parser/process_norm.c \
	parser/process_norm_2.c \
	utils/art.c \
	utils/utils.c \
	utils/utils_env.c \
	utils/expander_utils.c \
	utils/lexer_utils.c \
	utils/quote_handler_utils.c \
	utils/main_utils.c \
	utils/parser_utils.c \
	utils/exec_utils.c \
	utils/exec_utils_norm.c \
	utils/exe_utils.c \
	utils/builtin_utils.c \
	utils/clean.c \
	utils/built_utils.c \
	utils/cd_utils.c \
	utils/pwd_env.c\
	utils/builtin_util.c \
	utils/handle_utils.c \
	utils/expand_utils.c \
	utils/token_utils.c \
	utils/heredoc_utils.c \
	utils/executor_redir_utils.c \
	utils/utils_free.c \
	utils/utils_str.c \
	executor/exec.c \
	executor/exe.c \
	executor/executor.c \
	executor/executor_norm.c \
	executor/executor_norm_2.c \
	executor/signals.c \
	executor/executor_redir.c \
	executor/environment.c \
	parser/process.c \
	builtin/builtin.c \

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./ft_libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft -L/opt/homebrew/opt/readline/lib -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re