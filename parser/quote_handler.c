/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:53:59 by barto             #+#    #+#             */
/*   Updated: 2025/01/07 13:20:13 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*handle_quote_parsing(t_terminal *term, char *str)
{
	char	*result;
	int		i;
	
	result = alloc_mem(1);
	i = 0;
	result[0] = '\0';
	while (str[i])
	{
		if (str[i] == '\'')
			result = process_single_quote(str, &i, result);
		else if (str[i] == '"')
			result = process_double_quote(term, str, &i, result);
		else
			i++;
	}
	return (result);
}

char	*process_double_quote(t_terminal *term, char *str, int *i, char *result)
{
	char	*quoted;
	char	*new_result;

	quoted = get_double_quote_content(term, str, i);
	if (!quoted)
		return (free(result), NULL);
	new_result = concat_strings(result, quoted);
	free(result);
	free(quoted);
	return (new_result);
}

char	*get_single_quote_content(char *str, int *i)
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
