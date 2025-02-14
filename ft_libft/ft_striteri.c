/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:16 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/05 14:59:34 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*ft)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !ft)
		return ;
	i = 0;
	while (s[i])
	{
		ft(i, &s[i]);
		++i;
	}
}
