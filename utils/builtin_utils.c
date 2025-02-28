/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:59:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/07 15:52:06 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *create_env_string(char *name, char *value)
{
    char *tmp;
    char *result;

    tmp = ft_strjoin(name, "=");
    if (!tmp)
        return (NULL);
    result = ft_strjoin(tmp, value);
    free(tmp);
    return (result);
}

int cmd_export(t_terminal *term, char **args)
{
    int i;

    if (!args[1])
        return (print_export_env(term));
    i = 1;
    while (args[i])
    {
        if (export_handle_arg(term, args[i]) != 0)
            return (1);
        i++;
    }
    return (0);
}

int cmd_unset(t_terminal *term, char **args)
{
    int i;
    int env_index;

    if (!args[1])
        return (0);
    i = 1;
    while (args[i])
    {
        env_index = find_env_var_index(term->new_env, args[i]);
        if (env_index >= 0)
        {
            free(term->new_env[env_index]);
            while (term->new_env[env_index])
            {
                term->new_env[env_index] = term->new_env[env_index + 1];
                env_index++;
            }
        }
        i++;
    }
    
    // Aggiorna anche l'ambiente nella struttura a lista collegata
    update_env_list_after_unset(term, args);
    
    return (0);
}

int cmd_env(t_terminal *term, char **args)
{
    int i;

    (void)args;
    i = 0;
    while (term->new_env[i])
    {
        if (ft_strchr(term->new_env[i], '='))
            ft_putendl_fd(term->new_env[i], 1);
        i++;
    }
    return (0);
}

int print_export_env(t_terminal *term)
{
    char **sorted_env;
    int env_size;
    int i;

    // Conta le variabili d'ambiente
    env_size = 0;
    while (term->new_env[env_size])
        env_size++;
        
    // Duplica l'array per il sort
    sorted_env = alloc_mem(sizeof(char *) * (env_size + 1));
    i = 0;
    while (i < env_size)
    {
        sorted_env[i] = ft_strdup_safe(term->new_env[i]);
        i++;
    }
    sorted_env[env_size] = NULL;
    
    // Ordina l'array
    sort_env_array(sorted_env, env_size);
    
    // Stampa le variabili d'ambiente in formato export
    i = 0;
    while (sorted_env[i])
    {
        print_export_var(sorted_env[i]);
        free(sorted_env[i]);
        i++;
    }
    free(sorted_env);
    return (0);
}

int handle_export_error(char *name, char *value)
{
    display_error("export: not a valid identifier");
    free(name);
    free(value);
    return (1);
}

void update_env_list_after_unset(t_terminal *term, char **args)
{
    t_environment *current;
    t_environment *next;
    int i;
    
    i = 1;
    while (args[i])
    {
        current = term->env;
        while (current)
        {
            next = current->next;
            if (ft_strcmp(current->name, args[i]) == 0)
            {
                // Rimuovi il nodo dalla lista
                if (current->prev != current)  // Non è l'unico nodo
                {
                    if (current == term->env)
                        term->env = current->next;
                    
                    if (current->prev)
                        current->prev->next = current->next;
                    if (current->next)
                        current->next->prev = current->prev;
                }
                else  // È l'unico nodo
                {
                    term->env = NULL;
                }
                
                // Libera la memoria
                free(current->var);
                free(current->name);
                if (current->value)
                    free(current->value);
                free(current);
                break;
            }
            current = next;
        }
        i++;
    }
}
