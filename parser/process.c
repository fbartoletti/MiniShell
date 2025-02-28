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

t_argument *process_quote(char *input, int *i, char quote, t_argument **args)
{
    t_argument *arg;
    char *value;
    int start = *i + 1;
    int j = start;
    
    // Trova la fine della stringa tra virgolette
    while (input[j] && input[j] != quote)
        j++;
        
    if (!input[j])  // Virgoletta non chiusa
        return (NULL);
        
    // Crea una copia della stringa tra virgolette
    value = ft_strdup_safe(input + start);
    value[j - start] = '\0';
    
    // Crea un nuovo token
    arg = create_arg_token(FALSE, value);
    if (!arg)
    {
        free(value);
        return (NULL);
    }
    
    // Imposta il tipo di virgolette
    arg->quote.none = FALSE;
    if (quote == '\'')
        arg->quote.single = TRUE;
    else
        arg->quote.dbl = TRUE;
    
    *i = j + 1;  // Aggiorna l'indice dopo la virgoletta chiusa
    add_arg_token(args, arg);
    
    return (arg);
}

t_argument *process_special(char *input, int *i, t_argument **args)
{
    t_argument *arg;
    char *value;
    char c = input[*i];
    t_boolean is_pipe = (c == '|');
    t_boolean is_redir = (c == '<' || c == '>');
    
    // Gestisce token di 2 caratteri (<<, >>)
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
    
    // Crea un nuovo token
    arg = create_arg_token(TRUE, value);
    if (!arg)
    {
        free(value);
        return (NULL);
    }
    
    // Imposta il tipo di token
    if (is_pipe)
        arg->token.is_pipe = TRUE;
    else if (c == '<')
    {
        if (value[1] == '<')  // 
            arg->token.is_heredoc = TRUE;
        else  // 
            arg->token.is_infile = TRUE;
    }
    else if (c == '>')
    {
        if (value[1] == '>')  // >>
            arg->token.is_append = TRUE;
        else  // >
            arg->token.is_outfile = TRUE;
    }
    
    add_arg_token(args, arg);
    return (arg);
}

t_argument *process_word(char *input, int *i, t_argument **args)
{
    t_argument *arg;
    char *value;
    int start = *i;
    
    // Trova la fine della parola
    while (input[*i] && !is_whitespace(input[*i]) && 
           input[*i] != '\'' && input[*i] != '"' && 
           !is_special_char(input[*i]))
        (*i)++;
    
    // Crea una copia della parola
    value = alloc_mem(*i - start + 1);
    if (!value)
        return (NULL);
    
    strncpy(value, input + start, *i - start);
    value[*i - start] = '\0';
    
    // Crea un nuovo token
    arg = create_arg_token(FALSE, value);
    if (!arg)
    {
        free(value);
        return (NULL);
    }
    
    add_arg_token(args, arg);
    return (arg);
}
