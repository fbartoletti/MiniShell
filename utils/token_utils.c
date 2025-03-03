/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:29:56 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 08:43:46 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token_info	get_token_type(char *input, int i, int *len)
{
	t_token_info	token;

	ft_memset(&token, 0, sizeof(t_token_info));
	token.is_token = TRUE;
	if (input[i] == '<' && input[i + 1] == '<')
	{
		*len = 2;
		token.is_heredoc = TRUE;
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		*len = 2;
		token.is_append = TRUE;
	}
	else if (input[i] == '<')
		token.is_infile = TRUE;
	else if (input[i] == '>')
		token.is_outfile = TRUE;
	else
		token.is_pipe = TRUE;
	return (token);
}

int	create_special_token(t_argument **args, char *value, t_token_info type)
{
	t_argument	*new_token;

	if (!value)
		return (0);
	new_token = create_arg_token(TRUE, value);
	if (!new_token)
		return (0);
	new_token->token = type;
	add_arg_token(args, new_token);
	return (1);
}

int	handle_heredoc_delimiter(char *input, int *i, t_argument **args)
{
	int			start;
	char		*value;
	t_boolean	quote_mode;

	(*i) += 2;
	while (input[*i] && is_whitespace(input[*i]))
		(*i)++;
	start = *i;
	quote_mode = (input[*i] == '\'');
	if (quote_mode)
		(*i)++;
	while (input[*i] && ((quote_mode && input[*i] != '\'') || 
		(!quote_mode && !is_whitespace(input[*i]))))
		(*i)++;
	if (quote_mode && input[*i] == '\'')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (0);
	t_argument *arg = create_arg_token(FALSE, value);
	if (!arg)
	{
		free(value);
		return (0);
	}
	if (quote_mode)
	{
		arg->quote.none = FALSE;
		arg->quote.single = TRUE;
	}
	add_arg_token(args, arg);
	return (1);
}
