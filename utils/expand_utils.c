/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:49:38 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 10:48:32 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	expand_command_args(t_terminal *term, t_command_info *cmd)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->matrix[i])
	{
		expanded = expand_vars(term, cmd->matrix[i]);
		if (expanded)
		{
			free(cmd->matrix[i]);
			cmd->matrix[i] = expanded;
		}
		i++;
	}
	return (0);
}

void	execute_child_process(t_terminal *term, t_command_info *cmd)
{
	ignore_signals();
	setup_input_redirects(cmd->redirects);
	setup_output_redirects(cmd->redirects);
	if (!cmd->matrix || !cmd->matrix[0])
		exit(0);
	if (cmd->builtin.is_builtin)
	{
		execute_builtin_command(term, cmd);
		exit(g_last_status);
	}
	run_external_command(term, cmd);
	exit(127);
}
