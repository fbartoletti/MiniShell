/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process-utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiampa <fgiampa@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 02:06:04 by fgiampa           #+#    #+#             */
/*   Updated: 2025/03/11 02:06:15 by fgiampa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_argument	*handling_arg(char quote, char *value)
{
	t_argument	*arg;
	char		*temp;

	if (quote == '\'')
	{
		temp = value;
		value = ft_strjoin("\1", value);
		free(temp);
		if (!value)
			return (NULL);
	}
	arg = create_arg_token(FALSE, value);
	if (!arg)
	{
		free(value);
		return (NULL);
	}
	arg->quote.none = FALSE;
	if (quote == '\'')
		arg->quote.single = TRUE;
	else
		arg->quote.dbl = TRUE;
	return (arg);
}

t_argument	*case_1_helper(char *value, int *i, char *input)
{
	t_argument	*arg;
	int			start;
	int			quote_mode;

	start = *i;
	quote_mode = (input[*i] == '\'');
	if (quote_mode)
		(*i)++;
	while (input[*i] && ((quote_mode && input[*i] != '\'')
			|| (!quote_mode && !is_whitespace(input[*i])
				&& !is_special_char(input[*i]))))
		(*i)++;
	if (quote_mode && input[*i] == '\'')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	arg = create_arg_token(FALSE, value);
	if (!arg)
	{
		free(value);
		return (NULL);
	}
	return (arg);
}

char	*check_token_len(int *token_len, char c, int *i)
{
	char	*value;

	if (*token_len == 2)
	{
		value = alloc_mem(3);
		value[0] = c;
		value[1] = c;
		value[2] = '\0';
		(*i) += 2;
	}
	else
	{
		value = alloc_mem(2);
		value[0] = c;
		value[1] = '\0';
		(*i)++;
	}
	return (value);
}

t_argument	*handle_arg(char *value,
t_token_info	*token_type, t_argument **args)
{
	t_argument	*arg;

	arg = create_arg_token(TRUE, value);
	if (!arg)
	{
		free(value);
		return (NULL);
	}
	arg->token = *token_type;
	add_arg_token(args, arg);
	return (arg);
}
