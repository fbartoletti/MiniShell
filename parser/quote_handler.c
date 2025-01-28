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

char	*handle_quotes(t_minishell *shell, char *str)
{
	char	*result;
	int	i;
	
	result = safe_malloc(1);
	i = 0;
	result[0] = '\0';
	while (str[i])
	{
		if (str[i] == '\'')
			result = handle_quote_single(str, &i, result);
		else if (str[i] == '"')
			result = handle_quote_double(shell, str, &i, result);
		else
			i++;
	}
	return (result);
}
