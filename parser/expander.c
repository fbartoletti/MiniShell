/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:52:55 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 13:59:41 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*extract_var_name(char *str, int *i)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	if (str[start] == '{')
	{
		start++;
		(*i)++;
		while (str[start + len] && str[start + len] != '}')
			len++;
		if (str[start + len] == '}')
			(*i)++;
	}
	else
	{
		while (str[start + len] && (ft_isalnum(str[start + len]) || 
			   str[start + len] == '_'))
			len++;
	}
	if (len == 0)
		return (NULL);
	*i += len;
	return (ft_substr(str, start, len));
}

char	*get_env_var_value(char **env, char *key)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !*key)
		return (ft_strdup_safe(""));
	if (ft_strcmp(key, "?") == 0)
		return (NULL);
	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
		{
			value = ft_strdup_safe(env[i] + key_len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup_safe(""));
}

char	*get_status_str(t_terminal *term)
{
	char	exit_str[12];

	(void)term;
	snprintf(exit_str, sizeof(exit_str), "%d", g_last_status);
	return (ft_strdup_safe(exit_str));
}

char	*expand_vars(t_terminal *term, char *str)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	if (str[0] == '\1')
		return (ft_strdup_safe(str + 1));
	result = ft_strdup_safe("");
	if (!result)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$')
			result = expand_variable(term, str, &i, result);
		else
			result = add_char(result, str[i++]);
	}
	return (result);
}

int	expand_cmd_args(t_terminal *term, t_command_info *cmd)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->matrix && cmd->matrix[i])
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
