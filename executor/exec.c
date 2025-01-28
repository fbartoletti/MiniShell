/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:35:21 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 18:03:59 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(char *cmd)
{
	static const char *builtins[] = {
		"echo", "cd", "pwd", "export", 
		"unset", "env", "exit", NULL
	};
	int	i;

	i = 0;
	while (builtins[i])
	{
		if (!ft_strcmp(cmd, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}

void	setup_pipes(t_executor *exec)
{
	if (exec->prev_pipe != -1)
	{
		dup2(exec->prev_pipe, STDIN_FILENO);
		close(exec->prev_pipe);
	}
	if (exec->pipe_fd[1] != -1)
	{
		dup2(exec->pipe_fd[1], STDOUT_FILENO);
		close(exec->pipe_fd[1]);
	}
	if (exec->pipe_fd[0] != -1)
		close(exec->pipe_fd[0]);
}

void	execute_external(t_minishell *shell, t_command *cmd)
{
	char	*cmd_path;

	if (!cmd->args[0])
		exit(0);
	cmd_path = find_command_path(shell, cmd->args[0]);
	check_command_path(cmd_path, cmd->args[0]);
	if (execve(cmd_path, cmd->args, shell->env) == -1)
	{
		free(cmd_path);
		handle_command_error(cmd->args[0]);
		exit(126);
	}
}

int	execute_builtin(t_minishell *shell, t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	if (!setup_builtin_redirections(cmd, &saved_stdin, &saved_stdout))
		return (1);
	if (!ft_strcmp(cmd->args[0], "echo"))
		ret = ft_echo(shell, cmd->args);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		ret = ft_cd(shell, cmd->args);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		ret = ft_pwd(shell, cmd->args);
	else if (!ft_strcmp(cmd->args[0], "export"))
		ret = ft_export(shell, cmd->args);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		ret = ft_unset(shell, cmd->args);
	else if (!ft_strcmp(cmd->args[0], "env"))
		ret = ft_env(shell, cmd->args);
	else
		ret = ft_exit(shell, cmd->args);
	restore_redirections(saved_stdin, saved_stdout);
	return (ret);
}

void	handle_redirections(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (setup_redirection(redir) < 0)
			exit(1);
		redir = redir->next;
	}
}
