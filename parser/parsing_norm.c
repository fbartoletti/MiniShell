/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiampa <fgiampa@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 02:06:51 by fgiampa           #+#    #+#             */
/*   Updated: 2025/03/11 02:06:59 by fgiampa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	processing(t_argument **current, t_terminal *term, t_command_info **cmd)
{
	int	ret;

	while ((*current))
	{
		ret = handle_token(term, (*current), (*cmd));
		if (ret == 0)
		{
			free_cmd_content((*cmd));
			free((*cmd));
			return (0);
		}
		if (ret == 2 && (*current)->next)
			(*current) = (*current)->next->next;
		else if (ret == 1 && (*current)->token.is_pipe)
		{
			(*cmd) = create_cmd();
			if (!(*cmd))
				return (0);
			(*current) = (*current)->next;
		}
		else
			(*current) = (*current)->next;
	}
	return (1);
}
