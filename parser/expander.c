/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:52:55 by barto             #+#    #+#             */
/*   Updated: 2025/02/05 14:04:37 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_var_name(char *str, int *i)
{
	int	start;
	int	len;

	start = *i + 1;
	len = 0;
	while (str[start + len] && (isalnum(str[start + len]) || str[start + len] == '_'))
		len++;
	*i += len;
	return (ft_substr(str, start, len));
}

char	*get_env_value(char **env, char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (ft_strdup(env[i] + key_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*get_exit_status(t_minishell *shell)
{
	char	exit_str[12];
	
	snprintf(exit_str, sizeof(exit_str), "%d", shell->exit_status);
	return (ft_strdup(exit_str));
}

char	*handle_expansion(t_minishell *shell, char *str, int *i)
{
	char	*var_name;
	char	*value;

	if (str[*i + 1] == '?')
	{
		(*i)++;
		return (get_exit_status(shell));
	}
	var_name = get_var_name(str, i);
	value = get_env_value(shell->env, var_name);
	free(var_name);
	return (value);
}

char	*expand_variables(t_minishell *shell, char *str)
{
	char	*result;
	int	i;
	
	result = safe_malloc(1);
	i = 0;
	result[0] = '\0';
	while (str[i])
	{
		if (str[i] == '$')
			result = expand_var(shell, str, &i, result);
		else
			result = append_char(result, str[i]);
		i++;
	}
	return (result);
}
