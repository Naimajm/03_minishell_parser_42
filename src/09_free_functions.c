/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_free_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:55 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/14 14:51:47 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tokens_list(t_token **token_list);
void	free_matrix(char **matrix);

void	cleanup_minishell(t_shell *shell)
{
	if (!shell)
		return ;
	// free lista words
	if (shell->words_list)
		free_words_list(&shell->words_list);

	// free copia variables entorno
	if (shell->environment)
		free_matrix(shell->environment);

	free(shell);
	shell = NULL;
	ft_printf(FREE_ALL_SHELL);
}

void	free_words_list(t_word_token **words_list)
{
	t_word_token	*current_node;
	t_word_token	*next_node;

	if (!words_list)
        return ;

	current_node = (t_word_token *) *words_list;
	while (current_node)
	{
		next_node = current_node->next;

		// Liberar strings
		if (current_node->raw_word)
			free(current_node->raw_word);
		if (current_node->processed_word)
			free(current_node->processed_word);

		// Liberar lista de tokens
		if (current_node->tokens_list)
			free_tokens_list(&current_node->tokens_list);

		// Liberar nodo actual
		free(current_node);
		current_node = next_node;
	}    
	*words_list = NULL;
    ft_printf(FREE_WORDS_LIST);
}

void	free_tokens_list(t_token **token_list)
{
	t_token	*current_node;
	t_token	*next_node;

	if (!token_list)
		return ;
	current_node = (t_token *) *token_list;
	while (current_node)
	{
		next_node = current_node->next;
		
		// Liberar strings
		if (current_node->raw_token) 		// borrar raw_token
			free(current_node->raw_token);		
		if (current_node->expanded_token) 	// borrar expanded_token
			free(current_node->expanded_token);
		if (current_node->noquotes_token) 	
			free(current_node->noquotes_token);

		// Liberar lista nodos expand		
		if (current_node->expand_list)
			free_expand_stack((*token_list)->expand_list);

		// Liberar nodo actual
		free(current_node);		
		current_node = next_node; 
	}
	*token_list = NULL;
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