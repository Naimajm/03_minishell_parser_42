/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils_builtins_01.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:32:33 by emcorona          #+#    #+#             */
/*   Updated: 2025/07/30 20:39:57 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

int		ft_strcmp(char *s1, char *s2); // TODO PASARLO A LA LIBRERIA
char	*ft_find_plus_pos(char *var);


int	ft_strcmp( char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char *ft_find_plus_pos(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '+' && var[i + 1] == '=')
			return (var + i);
		i++;
	}
	return (NULL);
}