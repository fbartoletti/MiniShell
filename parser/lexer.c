/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:48:19 by barto             #+#    #+#             */
/*   Updated: 2025/02/12 18:29:47 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int i;
	
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '"')
		{
			if (!handle_quote(input, &i, input[i], &tokens))
				return (free_tokens(tokens), NULL);
		}
		else if (is_special_char(input[i]))
		{
			if (!handle_special(input, &i, &tokens))
				return (free_tokens(tokens), NULL);
		}
		else if (!handle_word(input, &i, &tokens))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;
    
    token = safe_malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    
    token->type = type;
    token->value = value;
    token->next = NULL;
    token->prev = NULL;  // Inizializza esplicitamente prev a NULL
    
    return (token);
}

void add_token(t_token **tokens, t_token *new)
{
    t_token *tmp;

    if (!tokens || !new)
        return;
        
    if (!*tokens)
        *tokens = new;
    else
    {
        tmp = *tokens;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
        new->prev = tmp;  // Aggiunge il collegamento al prev
    }
}
