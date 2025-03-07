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
	if (quote == '\'')
	{
		char *temp = value;
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
	{
		value = ft_strdup_safe("<<");
		arg = create_arg_token(TRUE, value);
		if (!arg)
		{
			free(value);
			return (NULL);
		}
		arg->token.is_heredoc = TRUE;
		add_arg_token(args, arg);
		*i += 2;
		while (input[*i] && is_whitespace(input[*i]))
			(*i)++;
		if (!input[*i] || input[*i] == '\0')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			return (NULL);
		}
		int start = *i;
		int quote_mode = (input[*i] == '\'');
		if (quote_mode)
			(*i)++;
		while (input[*i] && ((quote_mode && input[*i] != '\'') ||
				(!quote_mode && !is_whitespace(input[*i]) && !is_special_char(input[*i]))))
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
		add_arg_token(args, arg);
		return (arg);
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		token_len = 2;
		ft_memset(&token_type, 0, sizeof(t_token_info));
		token_type.is_token = TRUE;
		token_type.is_append = TRUE;
	}
	else if (input[*i] == '<')
	{
		ft_memset(&token_type, 0, sizeof(t_token_info));
		token_type.is_token = TRUE;
		token_type.is_infile = TRUE;
	}
	else if (input[*i] == '>')
	{
		ft_memset(&token_type, 0, sizeof(t_token_info));
		token_type.is_token = TRUE;
		token_type.is_outfile = TRUE;
	}
	else if (input[*i] == '|')
	{
		ft_memset(&token_type, 0, sizeof(t_token_info));
		token_type.is_token = TRUE;
		token_type.is_pipe = TRUE;
	}
	else
		return (NULL);
	if (token_len == 2)
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
	arg->token = token_type;
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
