/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:36:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/17 10:14:08 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_message_and_exit(char *message, int fd, int exit_code)
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