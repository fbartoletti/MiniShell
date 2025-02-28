/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:14:47 by barto             #+#    #+#             */
/*   Updated: 2025/01/08 11:53:04 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void *alloc_mem(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr)
        exit_with_error(ERR_MALLOC);
    return (ptr);
}

void exit_with_error(char *msg)
{
    ft_putstr_fd("Error: ", 2);
    ft_putstr_fd(msg, 2);
    ft_putstr_fd("\n", 2);
    exit(1);
}

void display_error(char *msg)
{
    ft_putstr_fd("Error: ", 2);
    ft_putstr_fd(msg, 2);
    ft_putstr_fd("\n", 2);
}

void free_arg_tokens(t_argument *args)
{
    t_argument *tmp;

    if (!args)
        return;
    
    if (args->prev == args)
    {
        // Lista con un solo elemento o lista circolare
        args->prev = NULL;
    }
    
    while (args)
    {
        tmp = args->next;
        free(args->str);
        free(args);
        args = tmp;
    }
}

void free_cmd_list(t_command_info *cmd)
{
    t_command_info *tmp;
    
    if (!cmd)
        return;
    
    if (cmd->prev == cmd)
        cmd->prev = NULL;
    while (cmd)
    {
        tmp = cmd->next;
        free_cmd_content(cmd);
        free(cmd);
        cmd = tmp;
    }
}

char *ft_strdup_safe(const char *s)
{
    char *dup;
    
    if (!s)
        return (NULL);
    
    dup = ft_strdup(s);
    if (!dup)
        exit_with_error(ERR_MALLOC);
    
    return (dup);
}

char *concat_strings(const char *s1, const char *s2)
{
    char *result;
    
    if (!s1 || !s2)
        return (NULL);
    
    result = ft_strjoin(s1, s2);
    if (!result)
        exit_with_error(ERR_MALLOC);
    
    return (result);
}

int str_compare(const char *s1, const char *s2)
{
    return (ft_strcmp(s1, s2));
}

void free_string_array(char **array)
{
    int i;
    
    if (!array)
        return;
    
    for (i = 0; array[i]; i++)
        free(array[i]);
    
    free(array);
}

void free_terminal(t_terminal *term)
{
    cleanup_memory(term);
    
    if (term->new_env)
    {
        free_string_array(term->new_env);
        term->new_env = NULL;
    }
    
    if (term->line)
    {
        free(term->line);
        term->line = NULL;
    }
    
    if (term->env)
    {
        free_env_list(&term->env);
    }
}
