/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_free_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:55 by juagomez          #+#    #+#             */
/*   Updated: 2025/06/11 21:12:57 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_token_list(t_token **token_list);
void	free_matrix(char **matrix);

void	cleanup_minishell(t_shell *shell)
{
	if (!shell)
		return ;
	// free lista tokens
	if (shell->token_list)
		free_token_list(&shell->token_list);
		
	// free copia variables entorno
	if (shell->environment)
		free_matrix(shell->environment);

	free(shell);
	shell = NULL;
	ft_printf(FREE_ALL_SHELL);
}

void	free_token_list(t_token **token_list)
{
	t_token	*temp_ptr;

	if (!token_list)
		return ;
	temp_ptr = *token_list; // establecer puntero 1º nodo inicio lista
	while (*token_list)
	{
		temp_ptr = temp_ptr->next;
		
		if ((*token_list)->raw_token) 		// borrar raw_token
			free((*token_list)->raw_token);
		
		if ((*token_list)->expanded_token) 	// borrar final_token
			free((*token_list)->expanded_token);

		if ((*token_list)->final_token)
			free((*token_list)->final_token);

		// Liberar lista de expansiones
		free_expand_stack((*token_list)->expand_list);
		
		free(*token_list);
		*token_list = NULL;
		// reestablecer puntero 1º nodo
		*token_list = temp_ptr;  
	}
	ft_printf(FREE_TOKENS);
}

void	free_expand_stack(t_expand *expand_stack)
{
	if (!expand_stack)
		return ;

	if (expand_stack->key)
		free(expand_stack->key);
	if (expand_stack->value)
		free(expand_stack->value);
	free(expand_stack);
	expand_stack = NULL;
}

void	free_matrix(char **matrix)
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