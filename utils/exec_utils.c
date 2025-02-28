/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:38:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/17 09:22:45 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *find_cmd_path(t_terminal *term, char *cmd)
{
    char    **paths;
    char    *path_env;
    char    *full_path;
    int     i;
    
    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
        return (ft_strdup_safe(cmd));
        
    path_env = get_env_from_array(term->new_env, "PATH");
    if (!path_env)
        return (NULL);
        
    paths = ft_split(path_env, ':');
    free(path_env);
    if (!paths)
        return (NULL);
        
    i = 0;
    while (paths[i])
    {
        char *temp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(temp, cmd);
        free(temp);
        
        if (access(full_path, X_OK) == 0)
        {
            free_string_array(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    
    free_string_array(paths);
    return (NULL);
}

int setup_redirection(t_redirect_node *redir, t_terminal *term)
{
    int fd;

    if (redir->type.is_infile)
    {
        fd = open(redir->fd_name, O_RDONLY);
        if (fd < 0)
            return (display_error("No such file or directory"), -1);
        dup2(fd, STDIN_FILENO);
    }
    else if (redir->type.is_heredoc)
    {
        fd = handle_heredoc_input(redir, term);
        if (fd < 0)
            return (-1);
        dup2(fd, STDIN_FILENO);
    }
    else if (redir->type.is_outfile)
    {
        fd = open(redir->fd_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
            return (display_error("Error opening output file"), -1);
        dup2(fd, STDOUT_FILENO);
    }
    else if (redir->type.is_append)
    {
        fd = open(redir->fd_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
            return (display_error("Error opening output file"), -1);
        dup2(fd, STDOUT_FILENO);
    }
    
    close(fd);
    return (0);
}

int handle_heredoc_input(t_redirect_node *redir, t_terminal *term)
{
    char    *content;
    int     pipe_fd[2];
    char    *real_delimiter;
    int     expand_mode;

    if (pipe(pipe_fd) < 0)
        return (-1);
        
    content = init_heredoc_data(redir->heredoc->delimiter, term, 
                                &real_delimiter, &expand_mode);
    if (!content)
        return (-1);
        
    content = read_heredoc_lines(content, real_delimiter, expand_mode, term);
    
    if (content)
    {
        write(pipe_fd[1], content, ft_strlen(content));
        free(content);
    }
    
    close(pipe_fd[1]);
    free_heredoc_data(expand_mode, real_delimiter, term);
    
    return (pipe_fd[0]);
}

char *get_env_from_array(char **env, const char *name)
{
    int name_len;
    int i;
    
    if (!name)
        return (NULL);
    
    name_len = ft_strlen(name);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
            return (ft_strdup(env[i] + name_len + 1));
        i++;
    }
    return (NULL);
}
