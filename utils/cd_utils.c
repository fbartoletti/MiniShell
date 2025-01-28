/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:24:43 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 13:27:12 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	change_directory(char *absolute_path, char *path, char *old_pwd)
{
	int	ret;

	ret = chdir(absolute_path);
	if (ret < 0)
	{
		free(absolute_path);
		free(path);
		return 1;
	}
	if (old_pwd)
	{
		setenv("OLDPWD", old_pwd, 1);
		free(old_pwd);
	}
	char *new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		setenv("PWD", new_pwd, 1);
		free(new_pwd);
	}
	free(absolute_path);
	free(path);
	return 0;
}
