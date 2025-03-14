/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:00:00 by fbartole          #+#    #+#             */
/*   Updated: 2025/03/13 15:24:45 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	create_env_node(t_terminal *term, char *var)
{
	t_environment	*node;
	char			*equal_sign;

	node = (t_environment *)alloc_mem(sizeof(t_environment));
	node->var = ft_strdup_safe(var);
	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
	{
		node->name = ft_substr(var, 0, equal_sign - var);
		node->value = ft_strdup_safe(equal_sign + 1);
		node->show = TRUE;
	}
	else
	{
		node->name = ft_strdup_safe(var);
		node->value = NULL;
		node->show = FALSE;
	}
	node->chain = FALSE;
	node->next = NULL;
	node->prev = NULL;
	add_env_node(&term->env, node);
}

void	add_env_node(t_environment **env, t_environment *new)
{
	t_environment	*last;

	if (!*env)
	{
		*env = new;
		new->prev = new;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
	(*env)->prev = new;
}

void	init_environment(t_terminal *term, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	term->new_env = alloc_mem((i + 1) * sizeof(char *));
	i = 0;
	while (env[i])
	{
		term->new_env[i] = ft_strdup_safe(env[i]);
		i++;
	}
	term->new_env[i] = NULL;
	term->env = NULL;
	i = 0;
	while (env[i])
	{
		create_env_node(term, env[i]);
		i++;
	}
}

void	update_shell_level(t_terminal *term)
{
	char	*shlvl_str;
	int		shlvl_val;
	char	*new_shlvl_str;

	shlvl_str = get_env_var(term->env, "SHLVL");
	if (!shlvl_str || !*shlvl_str)
		shlvl_val = 1;
	else
		shlvl_val = ft_atoi(shlvl_str) + 1;
	new_shlvl_str = ft_itoa(shlvl_val);
	if (!new_shlvl_str)
		return ;
	update_env_var(term, "SHLVL", new_shlvl_str);
	free(new_shlvl_str);
}
