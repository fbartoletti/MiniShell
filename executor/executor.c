/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 11:58:45 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_single_builtin(t_terminal *term)
{
	t_command_info	*cmd;

	cmd = term->commands;
	setup_input_redirects(cmd->redirects);
	setup_output_redirects(cmd->redirects);
	execute_builtin_command(term, cmd);
}

void run_commands(t_terminal *term)
{
    if (!term->commands)
        return;
        
    // Salva i descrittori di file standard
    term->stdin_copy = dup(STDIN_FILENO);
    term->stdout_copy = dup(STDOUT_FILENO);
    
    if (term->stdin_copy == -1 || term->stdout_copy == -1)
    {
        perror("dup");
        return;
    }
    
    // Caso speciale: un singolo comando builtin (non usiamo fork)
    if (term->commands && !term->commands->next && 
        term->commands->matrix && term->commands->matrix[0] && 
        is_builtin_cmd(term->commands->matrix[0]))
    {
        // Configura redirezioni
        setup_input_redirects(term->commands->redirects);
        setup_output_redirects(term->commands->redirects);
        
        // Esegui il builtin
        execute_builtin_command(term, term->commands);
    }
    else
    {
        // Segnali
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        
        // Esegui pipeline
        execute_pipeline(term);
        
        // Ripristina segnali
        signal(SIGINT, signal_handler);
        signal(SIGQUIT, SIG_IGN);
    }
    
    // Ripristina i descrittori standard
    restore_io(term);
}

void execute_pipeline(t_terminal *term)
{
    int pipe_fd[2];
    int prev_fd = -1;
    t_command_info *cmd;
    pid_t pid;
    int status;
    
    cmd = term->commands;
    while (cmd)
    {
        // Crea una pipe solo se c'è un comando successivo
        if (cmd->next && pipe(pipe_fd) < 0)
        {
            perror("pipe");
            if (prev_fd != -1)
                close(prev_fd);
            return;
        }
        
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            if (prev_fd != -1)
                close(prev_fd);
            if (cmd->next)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            return;
        }
        
        if (pid == 0) // Processo figlio
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            // Collega input dal precedente comando
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            
            // Collega output al comando successivo
            if (cmd->next)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            
            // Gestisci redirezioni specifiche del comando
            setup_input_redirects(cmd->redirects);
            setup_output_redirects(cmd->redirects);
            
            // Esegui comando
            if (cmd->matrix && cmd->matrix[0])
            {
                if (is_builtin_cmd(cmd->matrix[0]))
                {
                    execute_builtin_command(term, cmd);
                    exit(g_last_status);
                }
                else
                {
                    run_external_command(term, cmd);
                }
            }
            exit(0);
        }
        else
        {
            // Processo padre
            if (prev_fd != -1)
                close(prev_fd);
            
            if (cmd->next)
            {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            }
            
            cmd = cmd->next;
        }
    }
    
    // Attendi tutti i processi figli
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            g_last_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_last_status = 128 + WTERMSIG(status);
    }
}

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (1);
}
