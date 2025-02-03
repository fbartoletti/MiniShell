/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:36:43 by barto             #+#    #+#             */
/*   Updated: 2025/02/03 17:39:01 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_single_command(t_minishell *shell, t_command *cmd, t_executor *exec)
{
	if (!cmd->args || !cmd->args[0])
        return (0);

    // Se è un comando built-in, eseguilo direttamente
    if (is_builtin(cmd->args[0]) && !cmd->next)
        return (execute_builtin(shell, cmd));

    exec->pid = fork();
    if (exec->pid < 0)
    {
        perror("minishell: fork");
        return (-1);
    }

    if (exec->pid == 0) // Processo figlio
    {
        handle_child_signals();

        // Gestione pipe: input
        if (exec->prev_pipe != -1)
        {
            dup2(exec->prev_pipe, STDIN_FILENO);
            close(exec->prev_pipe); // Chiudi il descrittore di lettura
        }

        // Gestione pipe: output
        if (exec->pipe_fd[1] != -1)
        {
            dup2(exec->pipe_fd[1], STDOUT_FILENO);
            close(exec->pipe_fd[1]); // Chiudi il descrittore di scrittura
        }

        // Chiudi il descrittore di lettura della pipe nel processo figlio
        if (exec->pipe_fd[0] != -1)
            close(exec->pipe_fd[0]);

        // Gestisci redirezione se presente
        handle_redirections(cmd);

        // Esegui il comando built-in, se presente
        if (is_builtin(cmd->args[0]))
            exit(execute_builtin(shell, cmd));

        // Esegui un comando esterno
        execute_external(shell, cmd);
        exit(0); // Esci correttamente dopo aver eseguito il comando esterno
    }
    else // Processo padre
    {
        handle_parent_signals();

        // Chiudi i file descriptor nel processo padre che non sono più necessari
        if (exec->pipe_fd[1] != -1)
            close(exec->pipe_fd[1]);
        if (exec->prev_pipe != -1)
            close(exec->prev_pipe);

        // Attendi che il processo figlio termini
        waitpid(exec->pid, &exec->status, 0);
    }

    return (0); // Successo
}

void	handle_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	g_signal_received = 0;
}

void	handle_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}
