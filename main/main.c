/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:22:30 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 15:41:11 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal_received = 0;

void	handle_signal(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	init_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void init_minishell(t_minishell *shell, char **env)
{
	shell->env = env;
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	shell->in_heredoc = 0;
	init_signals();
}

static void	process_line(t_minishell *shell, char *line)
{
	int	status;

	if (!validate_and_tokenize(shell, line))
		return;
	status = parse_command(shell);
	if (status == 0)
	{
		print_error(ERR_PARSE);
		cleanup_process(shell);
		return;
	}
	execute_commands(shell);
	cleanup_process(shell);
}

int	main(int ac, char **av, char **env)
{
	t_minishell	shell;
	char	*line;
	
	(void)ac;
	(void)av;
	init_minishell(&shell, env);
	art();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break;
		process_line(&shell, line);
		free(line);
	}
	printf("\nexit shell ...\n");
	return (shell.exit_status);
}
