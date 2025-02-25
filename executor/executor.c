/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 15:54:28 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	executor(t_minishell *shell)
{
	    pid_t pid;
    int status;

    // Salva i file descriptor originali
    shell->stdin_copy = dup(STDIN_FILENO);
    shell->stdout_copy = dup(STDOUT_FILENO);
    if (shell->stdin_copy == -1 || shell->stdout_copy == -1)
    {
        print_error("dup error");
        return;
    }

    // Esegui builtin direttamente se è l'unico comando
    if (shell->commands && !shell->commands->next && 
        shell->commands->args && shell->commands->args[0] && 
        is_builtin(shell->commands->args[0]))
    {
        t_command *cmd = shell->commands;
        
        // Applica redirezioni
        handle_redirections(cmd, shell);
        shell->exit_status = execute_builtin(shell, cmd);
    }
    else
    {
        // Gestisci i segnali per l'esecuzione
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        
        // Crea un solo processo per tutta la pipeline
        pid = fork();
        if (pid == -1)
        {
            print_error("Fork failed");
            close(shell->stdin_copy);
            close(shell->stdout_copy);
            return;
        }
        
        if (pid == 0)
        {
            // Processo figlio gestisce tutte le pipe
            handle_pipeline(shell);
            // Non torna mai qui
        }
        
        // Aspetta che il processo pipeline termini
        waitpid(pid, &status, 0);
        
        // Ripristina i gestori di segnali interattivi
        signal(SIGINT, handle_signal);
        signal(SIGQUIT, SIG_IGN);
        
        shell->exit_status = handle_exit_status(status);
    }
    
    // Ripristina i file descriptor originali
    dup2(shell->stdin_copy, STDIN_FILENO);
    dup2(shell->stdout_copy, STDOUT_FILENO);
    close(shell->stdin_copy);
    close(shell->stdout_copy);
}

void	handle_pipeline(t_minishell *shell)
{
    t_command *current;
    int fd[2];
    pid_t pid;
    
    current = shell->commands;
    
    // Per gestire il caso in cui non ci sono comandi
    if (!current)
        exit(0);
        
    while (current)
    {
        if (pipe(fd) == -1)
        {
            print_error("Pipe error");
            exit(1);
        }
            
        pid = fork();
        if (pid == -1)
        {
            print_error("Fork error in pipeline");
            exit(1);
        }
            
        if (pid == 0)
        {
            // Processo figlio del figlio esegue il comando
            if (current->next)
                dup2(fd[1], STDOUT_FILENO);
                
            close(fd[0]);
            close(fd[1]);
            
            // Applica redirezioni
            handle_redirections(current, shell);
            
            // Esegui builtin o comando esterno
            if (current->args && current->args[0])
            {
                if (is_builtin(current->args[0]))
                {
                    int ret = execute_builtin(shell, current);
                    cleanup_child_process(shell);
                    exit(ret);
                }
                else
                {
                    execute_external(shell, current);
                    // execute_external già termina con exit
                }
            }
            
            cleanup_child_process(shell);
            exit(0);
        }
        else
        {
            // Processo figlio principale gestisce la pipeline
            close(fd[1]);
            
            if (current->next)
                dup2(fd[0], STDIN_FILENO);
                
            close(fd[0]);
            current = current->next;
        }
    }
    
    // Attendi tutti i processi figli
    while (wait(NULL) > 0)
        ;
        
    // Il processo pipeline termina qui
    cleanup_child_process(shell);
    exit(shell->exit_status);
}

int handle_exit_status(int status)
{
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);
    else
        return 1;
}
