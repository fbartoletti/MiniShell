/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:23:21 by barto             #+#    #+#             */
/*   Updated: 2025/02/11 14:35:05 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_quote(char *input, int *i, char quote, t_token **tokens)
{
	int		start;
	int		len;
	char	*value;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
		return (0);
	len = *i - start;
	value = ft_substr(input, start, len);
	if (!value)
		return (0);
	value = create_quoted_value(value, quote);
	if (!value)
		return (0);
	add_token(tokens, create_token(TOKEN_WORD, value));
	(*i)++;
	return (1);
}

int	handle_special(char *input, int *i, t_token **tokens)
{
	t_token_type	type;
	int				len;
	char			*value;

	if (!input || !i || !tokens)
		return (0);
	len = 1;
	type = get_token_type(input, *i, &len);
	value = ft_substr(input, *i, len);
	if (!create_special_token(tokens, value, type))
		return (0);
	*i += len;
	if (type == TOKEN_HEREDOC)
		return (handle_heredoc_delimiter(input, i, tokens));
	return (1);
}

int	handle_word(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*value;
	
	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && !is_special_char(input[*i]))
		(*i)++;
	value = ft_substr(input, start, *i - start);
	add_token(tokens, create_token(TOKEN_WORD, value));
	return (1);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || 
			c == '\v' || c == '\f' || c == '\r');
}
