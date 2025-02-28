/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:05:53 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 10:27:26 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command_info *create_cmd(void)
{
    t_command_info *cmd;
    
    cmd = alloc_mem(sizeof(t_command_info));
    if (!cmd)
        return (NULL);
    
    cmd->index = 0;
    cmd->matrix = NULL;
    cmd->redirects = NULL;
    cmd->builtin.is_builtin = FALSE;
    cmd->builtin.is_echo = FALSE;
    cmd->builtin.is_cd = FALSE;
    cmd->builtin.is_pwd = FALSE;
    cmd->builtin.is_export = FALSE;
    cmd->builtin.is_unset = FALSE;
    cmd->builtin.is_env = FALSE;
    cmd->builtin.is_exit = FALSE;
    cmd->next = NULL;
    cmd->prev = NULL;
    
    return (cmd);
}

void add_cmd(t_command_info **commands, t_command_info *new)
{
    t_command_info *tmp;

    if (!*commands)
    {
        *commands = new;
        new->prev = new;
        return;
    }
    tmp = *commands;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    new->prev = tmp;
    (*commands)->prev = new;
}

int is_redir_token(t_token_info token)
{
    return (token.is_infile || token.is_outfile || 
            token.is_append || token.is_heredoc);
}
