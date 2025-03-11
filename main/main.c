/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:22:30 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 13:35:38 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_last_status = 0;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_last_status = 1;
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

void	init_terminal(t_terminal *term, char **env)
{
	term->new_env = NULL;
	term->line = NULL;
	term->add_to_history = TRUE;
	term->args = NULL;
	term->commands = NULL;
	term->env = NULL;
	term->path = NULL;
	term->pipe_count = 0;
	term->error = FALSE;
	term->stdin_copy = -1;
	term->stdout_copy = -1;
	init_environment(term, env);
	init_signals();
}

static void	process_line(t_terminal *term, char *line)
{
	t_command_info	*cmd;

	if (!process_input_line(term, line))
		return ;
	cmd = term->commands;
	while (cmd)
	{
		expand_cmd_args(term, cmd);
		cmd = cmd->next;
	}
	if (term->error)
	{
		cleanup_memory(term);
		return ;
	}
	run_commands(term);
	cleanup_memory(term);
}

int	main(int ac, char **av, char **env)
{
	t_terminal	term;
	char		*line;

	init_terminal(&term, env);
	art();
	while (1 || !ac || !av)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("\nexit shell ...\n");
			break ;
		}
		if (line[0] != '\0')
		{
			term.line = line;
			process_line(&term, line);
			if (term.add_to_history)
				add_history(line);
		}
		free(line);
		line = NULL;
	}
	free_terminal(&term);
	return (g_last_status);
}
