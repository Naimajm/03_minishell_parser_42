/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.1_utils_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:36:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/20 22:34:42 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Función auxiliar para avanzar index segun la longitud del string
int	advance_index_by_length(int current_index, int length)
{
	if (length == FAILURE)
		perror_exit(ERROR_ADVANCE_INDEX, STDERR_FILENO, FAILURE);
	if (length == 0)
		current_index++;
	else	
		current_index += current_index + length;
	return (current_index);
}

void	perror_exit(char *message, int fd, int exit_code)
{
	if (message)
		ft_putendl_fd(message, fd);    
	exit(exit_code);
}

// Función auxiliar que libera automáticamente el primer parámetro
char *ft_strjoin_free(char *str1, char *str2)
{
	char *result;
	
	if (!str1 || !str2)
		return (NULL);
	result = ft_strjoin(str1, str2);
	free(str1);
	return (result);
}