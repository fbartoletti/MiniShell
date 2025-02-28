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

t_argument *tokenize_input(char *input)
{
	t_argument *args;
	int i;
	
	args = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '"')
		{
			if (!process_quote(input, &i, input[i], &args))
				return (free_arg_tokens(args), NULL);
		}
		else if (is_special_char(input[i]))
		{
			if (!process_special(input, &i, &args))
				return (free_arg_tokens(args), NULL);
		}
		else if (!process_word(input, &i, &args))
			return (free_arg_tokens(args), NULL);
	}
	return (args);
}

t_argument *create_arg_token(t_boolean is_token, char *value)
{
    t_argument *arg;
    
    arg = alloc_mem(sizeof(t_argument));
    if (!arg)
        return (NULL);
    
    arg->index = 0;
    arg->str = value;
    arg->token.is_token = is_token;
    arg->token.is_pipe = FALSE;
    arg->token.is_infile = FALSE;
    arg->token.is_outfile = FALSE;
    arg->token.is_append = FALSE;
    arg->token.is_heredoc = FALSE;
    arg->type.is_redirect = FALSE;
    arg->type.is_infile = FALSE;
    arg->type.is_outfile = FALSE;
    arg->type.is_append = FALSE;
    arg->type.is_heredoc = FALSE;
    arg->quote.none = TRUE;
    arg->quote.single = FALSE;
    arg->quote.dbl = FALSE;
    arg->chained = FALSE;
    arg->next = NULL;
    arg->prev = NULL;
    
    return (arg);
}

void add_arg_token(t_argument **args, t_argument *new)
{
    t_argument *tmp;

    if (!args || !new)
        return;
        
    if (!*args)
    {
        *args = new;
        new->prev = new;  // Il primo nodo punta a se stesso
    }
    else
    {
        tmp = *args;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
        new->prev = tmp;
        (*args)->prev = new;  // Ultimo nodo connesso al primo
    }
}
