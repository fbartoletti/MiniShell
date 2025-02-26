/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:16:23 by barto             #+#    #+#             */
/*   Updated: 2025/02/17 09:23:39 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_heredoc_redirect(t_redirect_node *node)
{
    int read_file;

    read_file = open(node->heredoc->temp_filename, O_RDONLY);
    if (read_file == -1)
    {
        ft_putstr_fd("Error on heredoc file opening.\n", 2);
        exit(1);
    }
    if (dup2(read_file, STDIN_FILENO) == -1)
        perror("dup2 heredoc");
    close(read_file);
}

void handle_input_redirect(t_redirect_node *node)
{
    int read_file;

    read_file = open(node->fd_name, O_RDONLY, 0777);
    if (read_file == -1)
    {
        ft_putstr_fd("Error on input file opening.\n", 2);
        exit(1);
    }
    if (dup2(read_file, STDIN_FILENO) == -1)
        perror("dup2 infile");
    close(read_file);
}

void handle_output_redirect(t_redirect_node *node)
{
    int write_file;

    write_file = open(node->fd_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (write_file == -1)
    {
        ft_putstr_fd("Error on output file opening.\n", 2);
        exit(1);
    }
    if (dup2(write_file, STDOUT_FILENO) == -1)
        perror("dup2 outfile");
    close(write_file);
}

void handle_append_redirect(t_redirect_node *node)
{
    int write_file;

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
