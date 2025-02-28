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

    if (!redir)
        return;
        
    // Rompi il ciclo nella lista circolare
    if (redir->prev == redir)
    {
        // Lista con un solo elemento
        if (redir->fd_name)
            free(redir->fd_name);
            
        if (redir->heredoc)
        {
            if (redir->heredoc->delimiter)
                free(redir->heredoc->delimiter);
            if (redir->heredoc->temp_filename)
            {
                if (access(redir->heredoc->temp_filename, F_OK) == 0)
                    unlink(redir->heredoc->temp_filename);
                free(redir->heredoc->temp_filename);
            }
            free(redir->heredoc);
        }
        
        free(redir);
        return;
    }
    
    // Lista con più elementi
    redir->prev->next = NULL;  // Rompi il ciclo
    
    while (redir)
    {
        tmp = redir->next;
        
        if (redir->fd_name)
            free(redir->fd_name);
            
        if (redir->heredoc)
        {
            if (redir->heredoc->delimiter)
                free(redir->heredoc->delimiter);
            if (redir->heredoc->temp_filename)
            {
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
            cmd->matrix[i] = NULL;  // Previeni use-after-free
            i++;
        }
        free(cmd->matrix);
        cmd->matrix = NULL;  // Previeni use-after-free
    }
    
    if (cmd->redirects)
    {
        free_redirect_node(cmd->redirects);
        cmd->redirects = NULL;  // Previeni use-after-free
    }
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

void cleanup_resources(void)
{
    // Attendi tutti i processi figli zombie
    while (waitpid(-1, NULL, WNOHANG) > 0);
    
    // Chiudi tutti i file descriptor aperti (opzionale, ma utile)
    int fd;
    for (fd = 3; fd < 1024; fd++)
        close(fd);
}
