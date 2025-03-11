/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process-utils-2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiampa <fgiampa@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 02:07:12 by fgiampa           #+#    #+#             */
/*   Updated: 2025/03/11 02:07:15 by fgiampa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_argument	*case_1(int *i, char *input, t_argument **args)
{
	t_argument	*arg;
	char		*value;

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
		ft_putstr_fd(ERR_TOK, 2);
		return (NULL);
	}
	arg = case_1_helper(value, i, input);
	if (!arg)
		return (NULL);
	add_arg_token(args, arg);
	return (arg);
}

void	case_2(t_token_info *token_type, int *token_len)
{
	*token_len = 2;
	ft_memset(token_type, 0, sizeof(t_token_info));
	(*token_type).is_token = TRUE;
	(*token_type).is_append = TRUE;
}

void	case_3(t_token_info *token_type)
{
	ft_memset(token_type, 0, sizeof(t_token_info));
	(*token_type).is_token = TRUE;
	(*token_type).is_infile = TRUE;
}

void	case_4(t_token_info *token_type)
{
	ft_memset(token_type, 0, sizeof(t_token_info));
	(*token_type).is_token = TRUE;
	(*token_type).is_outfile = TRUE;
}

void	case_5(t_token_info *token_type)
{
	ft_memset(token_type, 0, sizeof(t_token_info));
	(*token_type).is_token = TRUE;
	(*token_type).is_pipe = TRUE;
}
