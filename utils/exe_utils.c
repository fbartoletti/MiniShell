/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/02/17 09:23:39 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *read_heredoc_content(char *delimiter, t_terminal *term)
{
    char    *content;
    char    *line;
    int     quote_mode;
    char    *real_delimiter;

    content = ft_strdup_safe("");
    if (!content)
        return (NULL);
    quote_mode = (delimiter[0] == '\'' && 
                 delimiter[ft_strlen(delimiter) - 1] == '\'');
    if (quote_mode)
        real_delimiter = ft_strtrim(delimiter, "'");
    else
        real_delimiter = ft_strdup_safe(delimiter);
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, real_delimiter) == 0)
        {
            free(line);
            break;
        }
        if (!quote_mode && ft_strchr(line, '$'))
        {
            char *expanded = expand_vars(term, line);
            free(line);
            line = expanded;
        }
        content = append_to_content(content, line);
        free(line);
        if (!content)
        {
            free(real_delimiter);
            return (NULL);
        }
    }
    free(real_delimiter);
    return (content);
}

int process_heredoc_delimiter(char *input, int *i, t_argument **args)
{
    int         start;
    char        *value;
    t_argument  *arg;
    int         quote_mode;

    (*i) += 2;
    while (input[*i] && is_whitespace(input[*i]))
        (*i)++;
    start = *i;
    quote_mode = (input[*i] == '\'');
    if (quote_mode)
        (*i)++;
    while (input[*i] && ((quote_mode && input[*i] != '\'') || 
        (!quote_mode && !is_whitespace(input[*i]))))
        (*i)++;
    if (quote_mode && input[*i] == '\'')
        (*i)++;
    value = ft_substr(input, start, *i - start);
    if (!value)
        return (0);
    
    arg = create_arg_token(FALSE, value);
    if (!arg)
    {
        free(value);
        return (0);
    }
    
    if (quote_mode)
    {
        arg->quote.none = FALSE;
        arg->quote.single = TRUE;
    }
    
    add_arg_token(args, arg);
    return (1);
}

void handle_cmd_error(char *cmd)
{
    struct stat st;
    char        *error;

    if (stat(cmd, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
            error = "is a directory";
        else if (!(st.st_mode & S_IXUSR))
            error = "permission denied";
        else
            error = "command not found";
    }
    else
        error = "no such file or directory";
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(error, 2);
}

void check_cmd_exists(char *cmd_path, char *cmd)
{
    if (!cmd_path)
    {
        handle_cmd_error(cmd);
        exit(127);
    }
}

char *append_to_content(char *content, char *line)
{
    char *temp;
    char *new_content;

    temp = content;
    new_content = concat_strings(content, line);
    free(temp);
    if (!new_content)
        return (NULL);
    temp = new_content;
    new_content = concat_strings(new_content, "\n");
    free(temp);
    return (new_content);
}
