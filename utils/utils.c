/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:14:47 by barto             #+#    #+#             */
/*   Updated: 2025/01/08 11:53:04 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		error_exit(ERR_MALLOC);
	return (ptr);
}

void	error_exit(char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

void	print_error(char *msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	
	while (cmd)
	{
		tmp = cmd->next;
		free_command(cmd);
		free(cmd);
		cmd = tmp;
	}
}
