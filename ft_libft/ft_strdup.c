/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:12 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/06 17:05:54 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// prende una stringa e la copia para para
#include "libft.h"

char	*ft_strdup(const char *str)
{
	size_t	dup_sz;
	char	*dup_str;

	dup_sz = ft_strlen(str) + 1;
	dup_str = (char *)malloc(dup_sz);
	if (!dup_str)
		return (0);
	ft_memcpy(dup_str, str, dup_sz);
	return (dup_str);
}
