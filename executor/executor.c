/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/02/03 17:26:45 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_pipe_error(t_minishell *shell)
{
	ft_putendl_fd("minishell: pipe error", 2);
	shell->exit_status = 1;
}

int	prepare_pipes(t_executor *exec, t_command *current, t_minishell *shell)
{
	if (current->next)
	{
		if (pipe(exec->pipe_fd) < 0)
		{
			handle_pipe_error(shell);
			return (0);
		}
	}
	else
	{
		exec->pipe_fd[0] = -1;
		exec->pipe_fd[1] = -1;
	}
	return (1);
}

int	execute_commands(t_minishell *shell)
{
  	t_command *current;
    t_executor exec;
    int ret;

    current = shell->commands;
    init_executor(&exec);  // Inizializza la struttura exec
    ret = 0;

    while (current && ret >= 0)
    {
        // Se c'è un altro comando successivo, crea una pipe
        if (current->next)
        {
            if (pipe(exec.pipe_fd) < 0)
            {
                ft_putendl_fd("minishell: pipe error", 2);
                return (-1);
            }
        }
        else
        {
            // Se è l'ultimo comando, non c'è bisogno di una pipe
            exec.pipe_fd[0] = -1;
            exec.pipe_fd[1] = -1;
        }

        // Creazione del processo figlio con fork()
        exec.pid = fork();  // Salva il PID nel campo exec.pid

        if (exec.pid == 0)  // Processo figlio
        {
            // Imposta gli estremi della pipe (se necessario)
            if (exec.prev_pipe != -1)
            {
                dup2(exec.prev_pipe, 0); // Redirigi l'input
                close(exec.prev_pipe);
            }
            if (exec.pipe_fd[1] != -1)
            {
                dup2(exec.pipe_fd[1], 1); // Redirigi l'output
                close(exec.pipe_fd[1]);
            }

            // Esegui il comando
            ret = execute_single_command(shell, current, &exec);
            exit(ret == 0 ? 0 : 1);  // Se exec fallisce, esci con errore
        }
        else if (exec.pid > 0)  // Processo padre
        {
            // Memorizza il PID del processo figlio nel comando corrente
            current->pid = exec.pid;

            if (exec.pipe_fd[1] != -1)
                close(exec.pipe_fd[1]);  // Chiudi la scrittura nella pipe nel padre

            if (exec.prev_pipe != -1)
                close(exec.prev_pipe);  // Chiudi la lettura dalla pipe precedente nel padre

            exec.prev_pipe = exec.pipe_fd[0];  // Passa il read-end alla pipe successiva
        }
        else
        {
            // Gestione dell'errore nel fork()
            ft_putendl_fd("minishell: fork error", 2);
            return (-1);
        }

        current = current->next;  // Passa al prossimo comando
    }

    // Attendi che tutti i processi figlio terminino
    wait_all_processes(shell, shell->commands);
    return ret;
}
