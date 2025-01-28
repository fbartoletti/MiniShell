/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   art.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 18:32:05 by barto             #+#    #+#             */
/*   Updated: 2025/01/07 10:40:09 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

size_t	ft_strcspn(const char *str, const char *reject)
{
	size_t i;
	size_t j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (reject[j] != '\0')
		{
			if (str[i] == reject[j])
			{
				return (i);
			}
			j++;
		}
		i++;
	}
	return (i);
}

void	art()
{
	printf("  __  __ ___ _   _ ___ ____  _   _ _____ _     _     \n");
	printf(" |  \\/  |_ _| \\ | |_ _/ ___|| | | | ____| |   | |    \n");
	printf(" | |\\/| || ||  \\| || |\\___ \\| |_| |  _| | |   | |    \n");
	printf(" | |  | || || |\\  || | ___) |  _  | |___| |___| |___ \n");
	printf(" |_|  |_|___|_| \\_|___|____/|_| |_|_____|_____|_____|\n");
	printf("                                                     \n");
	printf("Program execution in progress...\n");
}
