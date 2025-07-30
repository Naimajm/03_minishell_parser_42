/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.1_utils_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:36:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/30 17:35:41 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Función auxiliar para avanzar index segun la longitud del string
int	advance_index_by_length(int current_index, int length)
{
	if (length == -1)
	{
		printf("DEBUG- >advance_index_by_length() -> length == -1\n");
		ft_putendl_fd(ERROR_ADVANCE_INDEX, STDERR_FILENO);
		return (0);
		//return (current_index);
	}		
	if (length == 0)
		current_index++;
	else	
		current_index += length;
	return (current_index);
}

// Función auxiliar que libera automáticamente el primer parámetro
char *ft_strjoin_free(char *str1, char *str2)
{
	char *result;
	
	if (!str1 && !str2)
		return (NULL);	
	if (!str1)
        result = ft_strdup(str2);
    else if (!str2)
        result = ft_strdup(str1);
	else
		result = ft_strjoin(str1, str2);
		
	if (!result)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);	
	if (str1)				// Solo liberar str1 si no era NULL
		free(str1);		
	return (result);
}