/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 18:13:57 by fbartole          #+#    #+#             */
/*   Updated: 2025/02/27 10:31:24 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	export_env_var(t_terminal *term, const char *var_name, const char *value)
{
	char	*env_var;
	
	env_var = ft_strjoin(var_name, value);
	if (env_var)
	{
		char *args[3] = {"export", env_var, NULL};
		cmd_export(term, args);
		free(env_var);
	}
}

void	update_pwd_env(t_terminal *term, const char *old_pwd)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		export_env_var(term, "PWD=", pwd);
		export_env_var(term, "OLDPWD=", old_pwd);
		free(pwd);
	}
}
