/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:04:18 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 14:20:04 by ubuntu           ###   ########.fr       */
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

extern int	g_last_status;

/* All my struct */
typedef enum e_boolean
{
	FALSE = 0,
	TRUE = 1,
}							t_boolean;

typedef struct s_quotation
{
	t_boolean				none;
	t_boolean				single;
	t_boolean				dbl;
}							t_quotation;

typedef struct s_token_info
{
	t_boolean				is_token;
	t_boolean				is_pipe;
	t_boolean				is_infile;
	t_boolean				is_outfile;
	t_boolean				is_append;
	t_boolean				is_heredoc;
}							t_token_info;

typedef struct s_redirect
{
	t_boolean				is_redirect;
	t_boolean				is_infile;
	t_boolean				is_outfile;
	t_boolean				is_append;
	t_boolean				is_heredoc;
}							t_redirect;

typedef struct s_builtin_cmd
{
	t_boolean				is_builtin;
	t_boolean				is_echo;
	t_boolean				is_cd;
	t_boolean				is_pwd;
	t_boolean				is_export;
	t_boolean				is_unset;
	t_boolean				is_env;
	t_boolean				is_exit;
}							t_builtin_cmd;

typedef struct s_environment
{
	char					*var;
	char					*name;
	char					*value;
	t_boolean				show;
	t_boolean				chain;
	struct s_environment	*next;
	struct s_environment	*prev;
}							t_environment;

typedef struct s_heredoc_data
{
	char					*delimiter;
	char					*temp_filename;
	t_boolean				expand;
	int						index;
}							t_heredoc_data;

typedef struct s_redirect_node
{
	char					*fd_name;
	t_heredoc_data			*heredoc;
	int						heredoc_fd;
	t_redirect				type;
	struct s_redirect_node  *next;
	struct s_redirect_node	*prev;
}							t_redirect_node;

typedef struct s_command_info
{
	int						index;
	char					**matrix;
	t_redirect_node			*redirects;
	t_builtin_cmd			builtin;
	struct s_command_info	*next;
	struct s_command_info	*prev;
}							t_command_info;

typedef struct s_argument
{
	int						index;
	char					*str;
	t_token_info			token;
	t_redirect				type;
	t_quotation				quote;
	t_boolean				chained;
	struct s_argument		*next;
	struct s_argument		*prev;
}							t_argument;

typedef struct s_terminal
{
	char					**new_env;
	char					*line;
	t_boolean				add_to_history;
	t_argument				*args;
	t_command_info			*commands;
	t_environment			*env;
	char					**path;
	int						pipe_count;
	t_boolean				error;
	int						stdin_copy;
	int						stdout_copy;
}							t_terminal;

/* main.c and main_utils.c */
void			terminal_loop(t_terminal *term);
int				main(int ac, char **av, char **env);
void			init_terminal(t_terminal *term, char **env);
void			free_input(t_terminal *term);
void			free_terminal(t_terminal *term);
int				check_syntax(char *line);
int				process_input_line(t_terminal *term, char *line);
void			cleanup_memory(t_terminal *term);

/* art.c */
size_t			ft_strcspn(const char *str, const char *reject);
void			art();

/* signal.c */
void			setup_interactive_signals(void);
void			ignore_signals(void);
void			handle_exec_signals(int signum);
void			signal_handler(int signum);

/* parser.c */
void			process_input(t_terminal *term);
t_argument		*tokenize_input(char *input);
t_argument		*create_arg_token(t_boolean is_token, char *value);
void			add_arg_token(t_argument **args, t_argument *new);
int				handle_token(t_terminal *term, t_argument *token, t_command_info *cmd);
t_command_info	*create_cmd(void);
void			add_cmd(t_command_info **cmds, t_command_info *new);
void			process_redirections(t_command_info *cmd, t_argument *token);
t_redirect_node	*create_redirect(t_redirect type, char *file);
void			add_redirect(t_command_info *cmd, t_redirect_node *redirect);
int				is_redir_token(t_token_info token);
void			add_command_to_shell(t_terminal *term, t_command_info *cmd);
t_argument		*process_quote(char *input, int *i, char quote, t_argument **args);
t_argument		*process_special(char *input, int *i, t_argument **args);
t_argument		*process_word(char *input, int *i, t_argument **args);

/* expansion.c */
char			*expand_vars(t_terminal *term, char *str);
char			*get_env_var_value(char **env, char *key);
char			*get_status_str(t_terminal *term);
char			*extract_var_name(char *str, int *i);
char			*expand_variable(t_terminal *term, char *str, int *i, char *result);
char			*add_char(char *str, char c);
int				expand_cmd_args(t_terminal *term, t_command_info *cmd);

/* quotes.c */
char			*handle_quote_parsing(t_terminal *term, char *str);
char			*process_single_quote(char *str, int *i, char *result);
char			*process_double_quote(t_terminal *term, char *str, int *i, char *result);
char			*get_single_quote_content(char *str, int *i);
char			*get_double_quote_content(t_terminal *term, char *str, int *i);

/* utils.c */
void			*alloc_mem(size_t size);
void			exit_with_error(char *msg);
void			display_error(char *msg);
void			free_arg_tokens(t_argument *args);
void			free_cmd_list(t_command_info *cmds);
char			*ft_strdup_safe(const char *s);
char			*concat_strings(const char *s1, const char *s2);
int				str_compare(const char *s1, const char *s2);
void			add_to_history(t_terminal *term);
void			display_art(void);
int				is_special_char(char c);
int				is_whitespace(char c);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcpy(char *dest, const char *src);
int				process_arguments_to_commands(t_terminal *term);
void			process_expansions(t_terminal *term);
char			*generate_heredoc_filename(int index);
int				setup_redirection(t_redirect_node *redir, t_terminal *term);
char			*get_env_from_array(char **env, const char *name);
void			check_cmd_exists(char *cmd_path, char *cmd);
void			update_env_list_after_unset(t_terminal *term, char **args);
int				handle_export_error(char *name, char *value);
int				is_valid_identifier(const char *name);
int				extract_name_value(char *arg, char **name, char **value);
int				export_handle_arg(t_terminal *term, char *arg);
void			print_export_var(char *var);
int				print_export_env(t_terminal *term);
char			*create_env_string(char *name, char *value);
void			sort_env_array(char **env, int size);
int				ft_export_create_or_update_env(t_terminal *term, char *name, char *env_string);
int				ft_export_print_env(t_terminal *term);
int				handle_no_equal(t_terminal *term, char *name, char *value);
int				handle_error(char *name, char *value);
t_redirect_node	*create_heredoc_queue(void);
void			add_to_heredoc_queue(t_redirect_node **queue, char *delimiter);
char			*process_heredoc_queue(t_terminal *term, t_redirect_node *queue);
void			export_env_var(t_terminal *term, const char *var_name, const char *value);
void			update_pwd_env(t_terminal *term, const char *old_pwd);
char			*create_quoted_value(char *value, char quote);
void			execute_child_process(t_terminal *term, t_command_info *cmd);
int				expand_command_args(t_terminal *term, t_command_info *cmd);
int				handle_heredoc_delimiter(char *input, int *i, t_argument **args);
int				create_special_token(t_argument **args, char *value, t_token_info type);
t_token_info	get_token_type(char *input, int i, int *len);
void			handle_redirections(t_command_info *cmd);
int				execute_builtin(t_terminal *term, t_command_info *cmd);
void			setup_pipes(int prev_pipe, int *pipe_fd);
void			save_io_descriptors(int *saved_stdin, int *saved_stdout);
int				count_valid_commands(t_command_info *cmd);
void			wait_for_processes(int cmd_count);
void			init_pipe_fds(int *pipe_fd, int *prev_pipe);
void			lose_pipe_fds(int *pipe_fd, int prev_pipe);
int				change_directory(char *absolute_path, char *path, char *old_pwd);
int				is_numeric(char *str);

/* executor.c */
void			run_commands(t_terminal *term);
int				get_exit_code(int status);
void			execute_pipeline(t_terminal *term);
void			handle_single_builtin(t_terminal *term);

/* executor_redir.c */
void			restore_io(t_terminal *term);
void			setup_input_redirects(t_redirect_node *redir);
void			setup_output_redirects(t_redirect_node *redir);

/* executor_redir_utils.c */
void			handle_heredoc_redirect(t_redirect_node *node);
void			handle_input_redirect(t_redirect_node *node);
void			handle_output_redirect(t_redirect_node *node);
void			handle_append_redirect(t_redirect_node *node);

/* executor_command.c */
void			run_external_command(t_terminal *term, t_command_info *cmd);
char			*find_cmd_path(t_terminal *term, char *cmd);
int				is_builtin_cmd(char *cmd);
void			free_string_array(char **array);
void			handle_cmd_error(char *cmd);

/* heredoc.c */
int				handle_heredoc_input(t_redirect_node *redir);
char			*init_heredoc_data(char *delimiter, t_terminal *term,
					char **real_delimiter, int *expand_mode);
char			*process_heredoc_content(char *line, t_terminal *term, int expand_mode);
void			free_heredoc_data(int expand_mode, char *real_delimiter,
					t_terminal *term);
char			*read_heredoc_lines(char *content, char *real_delimiter,
					int expand_mode, t_terminal *term);
char			*append_to_content(char *content, char *line);
int				prepare_heredocs(t_terminal *term);
void			assign_heredoc_indices(t_terminal *term);
int				collect_heredocs_input(t_command_info *cmd);

/* builtin.c */
void			execute_builtin_command(t_terminal *term, t_command_info *cmd);
void			identify_builtin(t_command_info *cmd);
int				cmd_echo(t_terminal *term, char **args);
int				cmd_cd(t_terminal *term, char **args);
int				cmd_pwd(t_terminal *term, char **args);
int				cmd_exit(t_terminal *term, char **args);
int				cmd_export(t_terminal *term, char **args);
int				cmd_unset(t_terminal *term, char **args);
int				cmd_env(t_terminal *term, char **args);
int				ft_cd_handle_path(t_terminal *term, char **args, char **path, char **old_pwd);

/* environment.c */
void			init_environment(t_terminal *term, char **env);
void			create_env_node(t_terminal *term, char *var);
void			add_env_node(t_environment **env, t_environment *new);
char			*get_env_var(t_environment *env, const char *name);
void			update_env_var(t_terminal *term, const char *name, const char *value);
int				find_env_var_index(char **env, char *name);
void			add_new_env_var(t_terminal *term, char *var_string);
void			update_shell_level(t_terminal *term);
void			update_env_array(t_terminal *term);
void			free_env_list(t_environment **env);

/* cleanup.c */
void			free_redirect_node(t_redirect_node *redir);
void			free_cmd_content(t_command_info *cmd);
void			cleanup_child(t_terminal *term);
void			cleanup_resources(void);

#endif

		/* TO DO LIST */
// se faccio export vedere se la variabile e gia presente

	// sistemare del tutto la norma: 
// run_commands
// execute_pipeline
// main
// process_input_line
// process_quote
// process_special
// update_env_list_after_unset
// free_redirect_node
// process_heredoc_delimiter
// read_heredoc_content
// setup_redirection
// find_cmd_path
// process_arguments_to_commands
// create_redirect
// handle_heredoc_delimiter
// update_env_var
// handle_heredoc_input
// prepare_heredocs
// handle_heredoc_redirect
