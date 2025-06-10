/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:36:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/06/10 13:38:46 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


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