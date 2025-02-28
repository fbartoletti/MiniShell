/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:40:20 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 15:55:36 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int check_basic_syntax(char *line)
{
    int i;
    int pipe_start;
    
    i = 0;
    pipe_start = 1;
    while (line[i])
    {
        if (line[i] == '|')
        {
            if (pipe_start)
                return (1);
            pipe_start = 1;
        }
        else if (!is_whitespace(line[i]))
            pipe_start = 0;
        i++;
    }
    return (pipe_start && !is_whitespace(line[i - 1]));
}

int process_arguments_to_commands(t_terminal *term)
{
    t_argument *current;
    t_command_info *cmd;
    int ret;

    current = term->args;
    cmd = create_cmd();
    if (!cmd)
        return (0);
    
    while (current)
    {
        ret = handle_token(term, current, cmd);
        if (ret == 0)
        {
            free_cmd_content(cmd);
            free(cmd);
            return (0);
        }
        
        if (ret == 2 && current->next)
            current = current->next->next;
        else
            current = current->next;
    }
    
    if (cmd->matrix || cmd->redirects)
        add_cmd(&term->commands, cmd);
    else
    {
        free_cmd_content(cmd);
        free(cmd);
    }
    
    // Process expansions in all commands
    process_expansions(term);
    
    return (1);
}

void cleanup_memory(t_terminal *term)
{
    if (term->args)
    {
        free_arg_tokens(term->args);
        term->args = NULL;
    }
    
    if (term->commands)
    {
        free_cmd_list(term->commands);
        term->commands = NULL;
    }
    
    if (term->path)
    {
        free_string_array(term->path);
        term->path = NULL;
    }
}
