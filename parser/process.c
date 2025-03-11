/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:48:19 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 13:58:08 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_argument	*process_quote(char *input, int *i, char quote, t_argument **args)
{
	t_argument	*arg;
	char		*value;
	int			start;
	int			j;

	start = *i + 1;
	j = start;
	while (input[j] && input[j] != quote)
		j++;
	if (!input[j])
		return (NULL);
	value = ft_substr(input, start, j - start);
	if (!value)
		return (NULL);
	arg = handling_arg(quote, value);
	*i = j + 1;
	add_arg_token(args, arg);
	return (arg);
}

t_argument	*process_special(char *input, int *i, t_argument **args)
{
	t_argument		*arg;
	char			*value;
	char			c;
	int				token_len;
	t_token_info	token_type;

	c = input[*i];
	token_len = 1;
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (case_1(i, input, args));
	else if (input[*i] == '>' && input[*i + 1] == '>')
		case_2(&token_type, &token_len);
	else if (input[*i] == '<')
		case_3(&token_type);
	else if (input[*i] == '>')
		case_4(&token_type);
	else if (input[*i] == '|')
		case_5(&token_type);
	else
		return (NULL);
	value = check_token_len(&token_len, c, i);
	arg = handle_arg(value, &token_type, args);
	return (arg);
}

t_argument	*process_word(char *input, int *i, t_argument **args)
{
	t_argument	*arg;
	char		*value;
	int			start;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i])
		&& input[*i] != '\'' && input[*i] != '"'
		&& !is_special_char(input[*i]))
		(*i)++;
	value = alloc_mem(*i - start + 1);
	if (!value)
		return (NULL);
	strncpy(value, input + start, *i - start);
	value[*i - start] = '\0';
	arg = create_arg_token(FALSE, value);
	if (!arg)
	{
		free(value);
		return (NULL);
	}
	add_arg_token(args, arg);
	return (arg);
}
