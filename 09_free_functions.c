/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_free_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:55 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/05 12:55:25 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	free_matrix(void **matrix);

void	cleanup_minishell(t_shell *shell)
{
	if (!shell)
		return ;
		
	free(shell);
	shell = NULL;
	ft_printf(FREE_ALL_SHELL);
}

void	free_matrix(void **matrix)
{
	int row;

	if (!matrix)
		return ;	
	row 	= 0;
	// limpieza por filas
	while (matrix[row])
	{
		if (matrix[row])
		{
			free(matrix[row]);
			matrix[row] = NULL;
		}
		row++;
	}
	// limpieza matriz
	free(matrix);
	matrix = NULL;
	ft_printf(FREE_MATRIX);
}