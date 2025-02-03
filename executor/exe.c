/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:22:32 by barto             #+#    #+#             */
/*   Updated: 2025/02/03 17:37:45 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipes(t_executor *exec)
{
	if (exec->prev_pipe != -1)
	{
		close(exec->prev_pipe);
		exec->prev_pipe = -1;
	}
	if (exec->pipe_fd[0] != -1)
	{
		close(exec->pipe_fd[0]);
		exec->pipe_fd[0] = -1;
	}
	if (exec->pipe_fd[1] != -1)
	{
		close(exec->pipe_fd[1]);
		exec->pipe_fd[1] = -1;
	}
}

void	init_executor(t_executor *exec)
{
	exec->pipe_fd[0] = -1;
	exec->pipe_fd[1] = -1;
	exec->prev_pipe = -1;
	exec->pid = -1;
	exec->status = 0;
}

void	wait_all_processes(t_minishell *shell, t_command *cmd)
{
	int status;
    pid_t pid;

    while (cmd)
    {
        if (cmd->args && cmd->args[0])
        {
            pid = waitpid(-1, &status, 0);  // Aspetta qualsiasi processo figlio
            if (pid == -1)
            {
                perror("waitpid error");
            }
            else
            {
                if (WIFEXITED(status))
                    shell->exit_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    shell->exit_status = 128 + WTERMSIG(status);
            }
        }
        cmd = cmd->next;
    }
}

int	setup_builtin_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
	t_redir	*redir;
	
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (setup_redirection(redir) < 0)
		{
			dup2(*saved_stdin, STDIN_FILENO);
			dup2(*saved_stdout, STDOUT_FILENO);
			close(*saved_stdin);
			close(*saved_stdout);
			return (0);
		}
		redir = redir->next;
	}
	return (1);
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
