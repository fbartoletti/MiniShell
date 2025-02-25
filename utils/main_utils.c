/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:40:20 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 15:55:36 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_basic_syntax(char *line)
{
	int	i;
	int	pipe_start;
	
	i = 0;
	pipe_start = 1;
	while (line[i])
	{
		if (line[i] == '|')
		{
			if (pipe_start)
				return (1);
			pipe_start = 1;
		}
		else if (!is_whitespace(line[i]))
			pipe_start = 0;
		i++;
	}
	return (pipe_start && !is_whitespace(line[i - 1]));
}

int	validate_and_tokenize(t_minishell *shell, char *line)
{
	if (!line || ft_strlen(line) == 0)
		return (0);
	add_history(line);
	if (check_basic_syntax(line) != 0)
	{
		print_error(ERR_SYNTAX);
		return (0);
	}
	shell->tokens = tokenize(line);
	if (!shell->tokens)
	{
		print_error(ERR_TOKEN);
		return (0);
	}
	return (1);
}

void	cleanup_process(t_minishell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->commands)
	{
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

void	cleanup_child_process(t_minishell *shell)
{
	shell->tokens = NULL;
	shell->commands = NULL;
	if (shell->stdin_copy != -1)
		close(shell->stdin_copy);
	if (shell->stdout_copy != -1)
		close(shell->stdout_copy);
}
