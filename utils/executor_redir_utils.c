/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:16:23 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 14:07:26 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_heredoc_redirect(t_redirect_node *node)
{
	int	fd;

	if (node->heredoc_fd >= 0)
	{
		if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			exit(1);
		}
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
	else
	{
		fd = handle_heredoc_input(node);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: errore heredoc\n", 2);
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}

void	handle_input_redirect(t_redirect_node *node)
{
	int	fd;

	fd = open(node->fd_name, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->fd_name, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	handle_output_redirect(t_redirect_node *node)
{
	int	fd;
	
	fd = open(node->fd_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->fd_name, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	handle_append_redirect(t_redirect_node *node)
{
	int	write_file;

	write_file = open(node->fd_name, O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (write_file == -1)
	{
		ft_putstr_fd("Error on append file opening.\n", 2);
		exit(1);
	}
	if (dup2(write_file, STDOUT_FILENO) == -1)
		perror("dup2 append");
	close(write_file);
}
