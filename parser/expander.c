/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:52:55 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 11:46:07 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;

	start = *i;
	len = 0;
	while (str[start + len] && (ft_isalnum(str[start + len]) || 
		str[start + len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	
	*i += len;
	return (ft_substr(str, start, len));
}

char	*get_env_value(char **env, char *key)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !*key)
		return (ft_strdup(""));
	if (ft_strcmp(key, "?") == 0)
		return (NULL);
	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
		{
			value = ft_strdup(env[i] + key_len + 1);
			return (value);
		}
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
	if (str[*i] == '?')
	{
		(*i)++;
		return (get_exit_status(shell));
	}
	return (expand_env_var(shell, str, i));
}

char	*expand_variables(t_minishell *shell, char *str)
{
	char	*result;
	int		i;
	int		in_single_quotes;
	
	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	in_single_quotes = 0;
	while (str && str[i])
	{
		if (str[i] == '\'')
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '$' && !in_single_quotes)
			result = expand_var(shell, str, &i, result);
		else
			result = append_char(result, str[i]);
		i++;
	}
	return (result);
}
