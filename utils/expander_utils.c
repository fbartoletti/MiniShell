/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:24:58 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 11:46:23 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_var(t_minishell *shell, char *str, int *i, char *result)
{
	char	*expanded;
	char	*new_result;
	
	(*i)++;
	if (!str[*i] || str[*i] == ' ')
		return (append_char(result, '$'));
	
	if (str[*i] == '?')
	{
		expanded = get_exit_status(shell);
		(*i)++;
	}
	else
		expanded = expand_env_var(shell, str, i);
	
	if (!expanded)
		return (result);
	
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
	int		j;
	
	var_name = get_var_name(str, i);
	if (!var_name)
		return (ft_strdup(""));
	j = 0;
	while (shell->env[j])
	{
		if (ft_strncmp(shell->env[j], var_name, ft_strlen(var_name)) == 0 
			&& shell->env[j][ft_strlen(var_name)] == '=')
		{
			value = ft_strdup(shell->env[j] + ft_strlen(var_name) + 1);
			free(var_name);
			return (value);
		}
		j++;
	}
	free(var_name);
	return (ft_strdup(""));
}
