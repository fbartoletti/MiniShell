/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:23:21 by barto             #+#    #+#             */
/*   Updated: 2025/01/04 16:28:25 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_quote(char *input, int *i, char quote, t_token **tokens)
{
	int		start;
	int		j;
	char	*value;
	
	start = *i + 1;
	j = start;
	while (input[j] && input[j] != quote)
		j++;
	if (!input[j])
		return (print_error(ERR_QUOTE), 0);
	value = ft_substr(input, start, j - start);
	add_token(tokens, create_token(TOKEN_WORD, value));
	*i = j + 1;
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
    if (input[*i] == '<' && input[*i + 1] == '<')
        type = TOKEN_HEREDOC, len = 2;
    else if (input[*i] == '>' && input[*i + 1] == '>')
        type = TOKEN_REDIR_APPEND, len = 2;
    else if (input[*i] == '<')
        type = TOKEN_REDIR_IN;
    else if (input[*i] == '>')
        type = TOKEN_REDIR_OUT;
    else
        type = TOKEN_PIPE;
    value = ft_substr(input, *i, len);
    if (!value)
        return (0);
    add_token(tokens, create_token(type, value));
    *i += len;
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
