/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:46:14 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 10:41:59 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_redirect_node(t_redirect_node *redir)
{
    t_redirect_node *tmp;

    while (redir)
    {
        tmp = redir->next;
        free(redir->fd_name);
        
        if (redir->heredoc)
        {
            if (redir->heredoc->delimiter)
                free(redir->heredoc->delimiter);
            if (redir->heredoc->temp_filename)
            {
                // Rimuovi anche il file temporaneo se esiste
                if (access(redir->heredoc->temp_filename, F_OK) == 0)
                    unlink(redir->heredoc->temp_filename);
                free(redir->heredoc->temp_filename);
            }
            free(redir->heredoc);
        }
        
        free(redir);
        redir = tmp;
    }
}

void free_cmd_content(t_command_info *cmd)
{
    int i;

    if (!cmd)
        return;
        
    if (cmd->matrix)
    {
        i = 0;
        while (cmd->matrix[i])
        {
            free(cmd->matrix[i]);
            i++;
        }
        free(cmd->matrix);
    }
    
    free_redirect_node(cmd->redirects);
}

void free_env_list(t_environment **env)
{
    t_environment *current;
    t_environment *next;

    if (!*env)
        return;
        
    // Se la lista è circolare, rompi il ciclo
    if ((*env)->prev == *env)
        (*env)->prev = NULL;
        
    current = *env;
    while (current)
    {
        next = current->next;
        
        if (current->var)
            free(current->var);
        if (current->name)
            free(current->name);
        if (current->value)
            free(current->value);
            
        free(current);
        current = next;
    }
    
    *env = NULL;
}

void cleanup_child(t_terminal *term)
{
    // Evita la doppia liberazione della memoria nel processo figlio
    term->args = NULL;
    term->commands = NULL;
    
    // Chiudi i descrittori di file salvati
    if (term->stdin_copy != -1)
    {
        close(term->stdin_copy);
        term->stdin_copy = -1;
    }
    
    if (term->stdout_copy != -1)
    {
        close(term->stdout_copy);
        term->stdout_copy = -1;
    }
}
