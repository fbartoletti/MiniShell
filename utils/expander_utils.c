/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:24:58 by barto             #+#    #+#             */
/*   Updated: 2025/01/02 13:48:50 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_var(t_minishell *shell, char *str, int *i, char *result)
{
	char	*expanded;
	char	*new_result;
	
	expanded = handle_expansion(shell, str, i);
	new_result = ft_strjoin(result, expanded);
	free(result);
	free(expanded);
	return (new_result);
}

char	*append_char(char *str, char c)
{
	char	temp[2];
	char	*new_result;
	
	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(str, temp);
	free(str);
	return (new_result);
}

char	*expand_env_var(t_minishell *shell, char *str, int *i)
{
	char	*var_name;
	char	*value;
	
	var_name = get_var_name(str, i);
	value = get_env_value(shell->env, var_name);
	free(var_name);
	return (value);
}
