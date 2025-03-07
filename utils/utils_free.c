/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:13:07 by ubuntu            #+#    #+#             */
/*   Updated: 2025/02/28 14:14:23 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_arg_tokens(t_argument *args)
{
	t_argument	*tmp;

	if (!args)
		return;
	if (args->prev == args)
		args->prev = NULL;
	while (args)
	{
		tmp = args->next;
		free(args->str);
		free(args);
		args = tmp;
	}
}

void	free_cmd_list(t_command_info *cmd)
{
	t_command_info	*tmp;

	if (!cmd)
		return;
	if (cmd->prev == cmd)
	{
		free_cmd_content(cmd);
		free(cmd);
		return;
	}
	cmd->prev->next = NULL;
	while (cmd)
	{
		tmp = cmd->next;
		free_cmd_content(cmd);
		free(cmd);
		cmd = tmp;
	}
}
void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return;
	for (i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}

void	free_terminal(t_terminal *term)
{
	cleanup_memory(term);
	if (term->new_env)
	{
		free_string_array(term->new_env);
		term->new_env = NULL;
	}
	if (term->line)
		term->line = NULL;
	if (term->env)
		free_env_list(&term->env);
}

void	exit_with_error(char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}
