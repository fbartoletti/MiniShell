/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 15:54:28 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void handle_single_builtin(t_terminal *term);

void run_commands(t_terminal *term)
{
    pid_t pid;
    int status;

    term->stdin_copy = dup(STDIN_FILENO);
    term->stdout_copy = dup(STDOUT_FILENO);
    if (term->stdin_copy == -1 || term->stdout_copy == -1)
    {
        ft_putstr_fd("dup error\n", 2);
        return;
    }

    // Esegui builtin direttamente se è l'unico comando
    if (term->pipe_count == 0 && term->commands->builtin.is_builtin)
        handle_single_builtin(term);
    else
    {
        // Gestisci i segnali per l'esecuzione
        ignore_signals();
        
        // Crea un solo processo per tutta la pipeline
        pid = fork();
        if (pid == -1)
        {
            ft_putstr_fd("Fork failed\n", 2);
            close(term->stdin_copy);
            close(term->stdout_copy);
            return;
        }
        
        if (pid == 0)
        {
            // Processo figlio gestisce tutte le pipe
            execute_pipeline(term);
            // Non torna mai qui
        }
        
        // Aspetta che il processo pipeline termini
        waitpid(pid, &status, 0);
        
        // Ripristina i gestori di segnali
        setup_interactive_signals();
        
        g_last_status = get_exit_code(status);
    }
    
    // Ripristina i file descriptor originali
    restore_io(term);
}

static void handle_single_builtin(t_terminal *term)
{
    t_command_info *cmd;

    cmd = term->commands;
    setup_input_redirects(cmd->redirects);
    setup_output_redirects(cmd->redirects);
    execute_builtin_command(term, cmd);
}

void execute_pipeline(t_terminal *term)
{
    int fd[2];
    t_command_info *cmd;
    pid_t pid;

    cmd = term->commands;
    
    // Per gestire il caso in cui non ci sono comandi
    if (!cmd)
        exit(0);
        
    while (cmd)
    {
        if (pipe(fd) == -1)
        {
            ft_putstr_fd("Pipe error\n", 2);
            exit(1);
        }
            
        pid = fork();
        if (pid == -1)
        {
            ft_putstr_fd("Fork error in pipeline\n", 2);
            exit(1);
        }
            
        if (pid == 0)
        {
            // Processo figlio esegue il comando
            if (cmd->next)
                dup2(fd[1], STDOUT_FILENO);
                
            close(fd[0]);
            close(fd[1]);
            
            // Applica redirezioni
            setup_input_redirects(cmd->redirects);
            setup_output_redirects(cmd->redirects);
            
            // Esegui builtin o comando esterno
            if (cmd->matrix && cmd->matrix[0])
            {
                if (cmd->builtin.is_builtin)
                {
                    execute_builtin_command(term, cmd);
                    exit(g_last_status);
                }
                else
                    run_external_command(term, cmd);
            }
            
            exit(0);
        }
        else
        {
            // Processo padre gestisce la pipeline
            close(fd[1]);
            
            if (cmd->next)
                dup2(fd[0], STDIN_FILENO);
                
            close(fd[0]);
            cmd = cmd->next;
        }
    }
    
    // Aspetta tutti i processi figli
    while (wait(NULL) > 0)
        ;
        
    exit(g_last_status);
}

int get_exit_code(int status)
{
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);
    else
        return 1;
}
