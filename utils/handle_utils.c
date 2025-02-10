/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:14:23 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 11:14:40 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_error(char *name, char *value)
{
	print_error("export: not a valid identifier");
	free(name);
	free(value);
	return (1);
}

int	handle_no_equal(t_minishell *shell, char *name, char *value)
{
	if (find_env_index(shell->env, name) == -1)
		add_new_env_var(shell, ft_strdup(name));
	free(name);
	free(value);
	return (0);
}
