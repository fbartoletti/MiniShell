/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:35:21 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 11:33:12 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_builtin_cmd(char *cmd)
{
    if (!cmd)
        return (0);
        
    if (ft_strcmp(cmd, "echo") == 0 ||
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "pwd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "env") == 0 ||
        ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

void	setup_pipes(int prev_pipe, int *pipe_fd)
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void run_external_command(t_terminal *term, t_command_info *cmd)
{
    char *cmd_path;

    if (!cmd->matrix || !cmd->matrix[0])
        exit(0);
        
    cmd_path = find_cmd_path(term, cmd->matrix[0]);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->matrix[0], 2);
        ft_putendl_fd(": comando non trovato", 2);
        exit(127);
    }
    
    if (execve(cmd_path, cmd->matrix, term->new_env) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->matrix[0], 2);
        ft_putstr_fd(": ", 2);
        perror("");
        free(cmd_path);
        exit(126);
    }
    
    free(cmd_path);
    exit(1);
}

int	execute_builtin(t_terminal *term, t_command_info *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
		return (1);
	setup_input_redirects(cmd->redirects);
	setup_output_redirects(cmd->redirects);
	execute_builtin_command(term, cmd);
	ret = g_last_status;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (ret);
}

void	handle_redirections(t_command_info *cmd)
{
	t_redirect_node	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type.is_heredoc)
			handle_heredoc_redirect(redir);
		else if (redir->type.is_infile)
			handle_input_redirect(redir);
		else if (redir->type.is_outfile)
			handle_output_redirect(redir);
		else if (redir->type.is_append)
			handle_append_redirect(redir);
		redir = redir->next;
	}
}
