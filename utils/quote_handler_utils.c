/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:25:49 by barto             #+#    #+#             */
/*   Updated: 2025/01/02 13:50:12 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *process_single_quote(char *str, int *i, char *result)
{
    char *quoted;
    char *new_result;
    
    quoted = get_single_quote_content(str, i);
    if (!quoted)
        return (free(result), NULL);
    
    new_result = concat_strings(result, quoted);
    free(result);
    free(quoted);
    return (new_result);
}

char *get_double_quote_content(t_terminal *term, char *str, int *i)
{
    int start;
    int j;
    char *content;
    char *expanded;
    
    start = *i + 1;
    j = start;
    while (str[j] && str[j] != '"')
        j++;
    
    if (!str[j])
        return (NULL);
    
    content = ft_substr(str, start, j - start);
    if (!content)
        return (NULL);
        
    expanded = expand_vars(term, content);
    free(content);
    
    *i = j + 1;
    return (expanded);
}

// Per gestire l'evidenziazione speciale delle stringhe a virgolette singole
char *create_quoted_value(char *value, char quote)
{
    char *quoted_value;
    
    if (quote == '\'')
    {
        quoted_value = concat_strings("\1", value);
        free(value);
        if (!quoted_value)
            return (NULL);
        return (quoted_value);
    }
    return (value);
}
