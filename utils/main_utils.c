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

void	cleanup_memory(t_terminal *term)
{
	if (term->args)
	{
		free_arg_tokens(term->args);
		term->args = NULL;
	}
	if (term->commands)
	{
		free_cmd_list(term->commands);
		term->commands = NULL;
	}
	if (term->path)
	{
		free_string_array(term->path);
		term->path = NULL;
	}
}
