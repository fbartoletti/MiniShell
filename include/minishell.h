/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:04:18 by barto             #+#    #+#             */
/*   Updated: 2025/02/05 15:42:22 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../ft_libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <termios.h>
# include <ctype.h>

/* Error messages */
# define ERR_MALLOC		"Memory allocation failed"
# define ERR_SYNTAX		"Syntax error"
# define ERR_QUOTE		"Unclosed quote"
# define ERR_PIPE		"Invalid pipe"
# define ERR_REDIR		"Invalid redirection"
# define ERR_CMD		"Command not found"
# define ERR_TOKEN		"Tokenization error"
# define ERR_PARSE		"Pars error"

extern int	g_signal_received;

/* All my struct */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}   t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}   t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}   t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}   t_command;

typedef struct s_minishell
{
	char		**env;
	t_token		*tokens;
	t_command	*commands;
	int			exit_status;
	int			in_heredoc;
}   t_minishell;

typedef struct s_executor
{
	int		pipe_fd[2];
	int		prev_pipe;
	pid_t	pid;
	int		status;
} t_executor;

typedef struct s_builtin
{
    char	*name;
    int		(*func)(t_minishell *shell, char **args);
} t_builtin;

/* main.c and main_utils.c */
void		handle_signal(int sig);
void		init_minishell(t_minishell *shell, char **env);
int			main(int ac, char **av, char **env);
int			check_basic_syntax(char *line);
int			validate_and_tokenize(t_minishell *shell, char *line);
void		cleanup_process(t_minishell *shell);

/* lexer.c and lexer_utils.c */
t_token		*tokenize(char *input);
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **tokens, t_token *new);
int			is_special_char(char c);
int			is_whitespace(char c);
int			handle_quote(char *input, int *i, char quote, t_token **tokens);
int			handle_special(char *input, int *i, t_token **tokens);
int			handle_word(char *input, int *i, t_token **tokens);

/* parser.c and parsing.c and parser_utils.c */
int			parse_command(t_minishell *shell);
t_command	*create_command(void);
void		add_command(t_command **commands, t_command *new);
int			is_redir_token(t_token_type type);
void		handle_redirection(t_command *cmd, t_token *token);
void		free_command_content(t_command *cmd);
void		add_command_to_shell(t_minishell *shell, t_command *cmd);
int			parse_token(t_minishell *shell, t_token *token, t_command *cmd);
t_redir		*create_redirection(t_token_type type, char *file);
void		add_redir_to_cmd(t_command *cmd, t_redir *redir);

/* expander.c and expander_utils.c */
char		*expand_variables(t_minishell *shell, char *str);
char		*get_env_value(char **env, char *key);
char		*get_exit_status(t_minishell *shell);
char		*get_var_name(char *str, int *i);
char		*expand_var(t_minishell *shell, char *str, int *i, char *result);
char		*append_char(char *str, char c);
char		*handle_expansion(t_minishell *shell, char *str, int *i);
char		*expand_env_var(t_minishell *shell, char *str, int *i);

/* quote_handler.c and quote_handler_utils.c */
char		*handle_quotes(t_minishell *shell, char *str);
char 		*handle_quote_single(char *str, int *i, char *result);
char		*handle_quote_double(t_minishell *shell, char *str, int *i, char *result);
char		*extract_single_quote(char *str, int *i);
char		*extract_double_quote(t_minishell *shell, char *str, int *i);

/* utils.c */
void		*safe_malloc(size_t size);
void		error_exit(char *msg);
void		print_error(char *msg);
void		free_tokens(t_token *tokens);
void		free_commands(t_command *cmd);

/* utils_string.c */
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *s);
char		*ft_strcpy(char *dest, const char *src);
int			ft_strcmp(const char *s1, const char *s2);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strjoin(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

/* art.c */
void		art(void);

/* exec.c and exec_utils.c and exe_utils.c and exe.c and executor.c */
char		*find_command_path(t_minishell *shell, char *cmd);
int			setup_redirection(t_redir *redir);
void		handle_redirections(t_command *cmd);
void		setup_pipes(t_executor *exec);
int			is_builtin(char *cmd);
int			execute_builtin(t_minishell *shell, t_command *cmd);
void		free_array(char **array);
int			handle_heredoc(t_redir *redir);
int			execute_commands(t_minishell *shell);
void		execute_external(t_minishell *shell, t_command *cmd);
int			execute_commands(t_minishell *shell);
void		init_executor(t_executor *exec);
void		close_pipes(t_executor *exec);
void		wait_all_processes(t_minishell *shell, t_command *cmd);
void 		check_command_path(char *cmd_path, char *cmd);
void		handle_command_error(char *cmd);
void		restore_redirections(int saved_stdin, int saved_stdout);
int			setup_builtin_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout);

/* builtin.c and builtin_utils.c */
int			ft_echo(t_minishell *shell, char **args);
int			ft_cd(t_minishell *shell, char **args);
int			ft_cd_handle_path(t_minishell *shell, char **args, char **path, char **old_pwd);
int			ft_pwd(t_minishell *shell, char **args);
int			ft_exit(t_minishell *shell, char **args);
int			ft_export(t_minishell *shell, char **args);
int			ft_unset(t_minishell *shell, char **args);
int			ft_env(t_minishell *shell, char **args);
int 		ft_export_print_env(t_minishell *shell);
int			ft_export_create_or_update_env(t_minishell *shell, char *name, char *env_string);
int			ft_export_handle_arg(t_minishell *shell, char *arg);
char		*create_env_string(char *name, char *value);
int			find_env_index(char **env, char *name);
void 		add_new_env_var(t_minishell *shell, char *env_string);

/* signals.c */
int			execute_single_command(t_minishell *shell, t_command *cmd, t_executor *exec);
void		handle_child_signals(void);
void		handle_parent_signals(void);
void		restore_signals(void);

/* clean.c */
void		free_redir(t_redir *redir);
void		free_command(t_command *cmd);

/* built_utils.c and cd_utils.c */
char		*get_env_var(char **env, const char *name);
void		update_pwd_env(t_minishell *shell, const char *old_pwd);
int			change_directory(char *absolute_path, char *path, char *old_pwd);

#endif

/* TO DO LIST */
//sistemare pipe
//sistemare env e export mettere in ordine
//expander $
//heredoc
//exit con lettera mi deve uscire con 2
//exit con piu di 2 arg mi deve dare too many arg e rimanere sulla shell con codice uscita 2
//ridurre void	update_pwd_env(t_minishell *shell, const char *old_pwd)
//ridurre righe int	ft_export_handle_arg(t_minishell *shell, char *arg)