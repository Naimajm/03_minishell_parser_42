/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.1_utils_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:36:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/22 12:52:54 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Función auxiliar para avanzar index segun la longitud del string
int	advance_index_by_length(int current_index, int length)
{
	if (length == GENERAL_ERROR)
		print_error(ERROR_ADVANCE_INDEX, STDERR_FILENO, GENERAL_ERROR);
	if (length == 0)
		current_index++;
	else	
		current_index += length;
	return (current_index);
}

void	print_error(char *message, int fd, int exit_code)
{
	if (message)
		ft_putendl_fd(message, fd);    
	//exit(exit_code);
	//printf("exit_code-> %i\n", exit_code);
	(void)exit_code;
}

/* void	perror_exit(char *message, int fd, int exit_code)
{
	if (message)
		ft_putendl_fd(message, fd);    
	exit(exit_code);
} */

// Función auxiliar que libera automáticamente el primer parámetro
char *ft_strjoin_free(char *str1, char *str2)
{
	char *result;
	
	if (!str1 && !str2)
		return (NULL);

	// casos NULL 
	if (!str1)
        result = ft_strdup(str2);
    else if (!str2)
        result = ft_strdup(str1);
	else
		result = ft_strjoin(str1, str2);
		
	if (!result)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
		
	// Solo liberar str1 si no era NULL
	if (str1)
		free(str1);
		
	return (result);
}