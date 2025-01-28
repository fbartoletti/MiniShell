/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:25:49 by barto             #+#    #+#             */
/*   Updated: 2025/01/02 13:50:12 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*handle_quote_single(char *str, int *i, char *result)
{
	char	*quoted;
	char	*new_result;
	
	quoted = extract_single_quote(str, i);
	if (!quoted)
		return (free(result), NULL);
	new_result = ft_strjoin(result, quoted);
	free(result);
	free(quoted);
	return (new_result);
}

char	*handle_quote_double(t_minishell *shell, char *str, int *i, char *result)
{
	char	*quoted;
	char	*new_result;
	
	quoted = extract_double_quote(shell, str, i);
	if (!quoted)
		return (free(result), NULL);
	new_result = ft_strjoin(result, quoted);
	free(result);
	free(quoted);
	return (new_result);
}

char	*extract_single_quote(char *str, int *i)
{
	int	start;
	int	j;
	
	start = *i + 1;
	j = start;
	while (str[j] && str[j] != '\'')
		j++;
	if (!str[j])
		return (NULL);
	*i = j + 1;
	return (ft_substr(str, start, j - start));
}

char	*extract_double_quote(t_minishell *shell, char *str, int *i)
{
	int	start;
	int	j;
	char	*content;
	char	*expanded;
	
	start = *i + 1;
	j = start;
	while (str[j] && str[j] != '"')
		j++;
	if (!str[j])
		return (NULL);
	content = ft_substr(str, start, j - start);
	expanded = expand_variables(shell, content);
	free(content);
	*i = j + 1;
	return (expanded);
}
