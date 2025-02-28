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

int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
}
