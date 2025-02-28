/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:48:19 by barto             #+#    #+#             */
/*   Updated: 2025/02/28 14:20:05 by ubuntu           ###   ########.fr       */
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
	value = ft_strdup_safe(input + start);
	value[j - start] = '\0';
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
	*i = j + 1;
	add_arg_token(args, arg);
	return (arg);
}

t_argument	*process_special(char *input, int *i, t_argument **args)
{
	t_argument	*arg;
	char		*value;
	char		c;
	t_boolean	is_pipe;
	t_boolean	is_redir;

	c = input[*i];
	is_pipe = (c == '|');
	is_redir = (c == '<' || c == '>');
	if (is_redir && input[*i + 1] == c)
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
	arg = create_arg_token(TRUE, value);
	if (!arg)
	{
		free(value);
		return (NULL);
	}
	if (is_pipe)
		arg->token.is_pipe = TRUE;
	else if (c == '<')
	{
		if (value[1] == '<')
			arg->token.is_heredoc = TRUE;
		else
			arg->token.is_infile = TRUE;
	}
	else if (c == '>')
	{
		if (value[1] == '>')
			arg->token.is_append = TRUE;
		else
			arg->token.is_outfile = TRUE;
	}
	add_arg_token(args, arg);
	return (arg);
}

t_argument	*process_word(char *input, int *i, t_argument **args)
{
	t_argument	*arg;
	char		*value;
	int			start;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && 
		   input[*i] != '\'' && input[*i] != '"' && 
		   !is_special_char(input[*i]))
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
