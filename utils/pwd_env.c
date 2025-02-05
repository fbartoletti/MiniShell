/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 18:13:57 by fbartole          #+#    #+#             */
/*   Updated: 2025/02/05 18:19:21 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	export_env_var(t_minishell *shell, const char *var_name, const char *value)
{
	char	*env_var;
	
	env_var = ft_strjoin(var_name, value);
	if (env_var)
	{
		char *args[3] = {"export", env_var, NULL};
		ft_export(shell, args);
		free(env_var);
	}
}

void	update_pwd_env(t_minishell *shell, const char *old_pwd)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		export_env_var(shell, "PWD=", pwd);
		export_env_var(shell, "OLDPWD=", old_pwd);
		free(pwd);
	}
}
