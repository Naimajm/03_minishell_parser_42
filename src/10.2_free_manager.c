/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.2_free_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:55 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/21 23:03:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_commands_list(t_cmd **commands_list);
void	free_words_list(t_word **words_list);
void	free_tokens_list(t_token **token_list);
void	free_expands_list(t_expand *expand_stack);
void	free_matrix(char **matrix);

void	cleanup_minishell(t_shell *shell)
{
	if (!shell)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	
	if (shell->environment)						// free copia variables entorno
		free_matrix(shell->environment);

	if (shell->commands_list) 					// free lista comandos
		free_commands_list(&shell->commands_list);

	free(shell);
	shell = NULL;
	ft_printf(FREE_ALL_SHELL);
}

void	free_commands_list(t_cmd **commands_list)
{
	t_cmd	*current_node;
	t_cmd	*next_node;

	if (!commands_list)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	
	current_node = (t_cmd *) *commands_list;
	while (current_node)
	{
		next_node = current_node->next;

		free_matrix(current_node->args);

		// Liberar strings
		if (current_node->command)
			free(current_node->command);
		if (current_node->infile)
			free(current_node->infile);
		if (current_node->outfile)
			free(current_node->outfile);
		if (current_node->delimiter)
			free(current_node->delimiter);
		
		// Liberar lista words
		if (current_node->words_list)
			free_words_list(&current_node->words_list);
		
		free(current_node); 	// Liberar nodo actual
		current_node = next_node;
	}
	*commands_list = NULL;
	ft_printf(FREE_COMMANDS_LIST);	
}

void	free_words_list(t_word **words_list)
{
	t_word	*current_node;
	t_word	*next_node;

	if (!words_list)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);

	current_node = (t_word *) *words_list;
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

		free(current_node);		// Liberar nodo actual
		current_node = next_node;
	}    
	*words_list = NULL;
	ft_printf(FREE_WORDS_LIST);
}

void	free_tokens_list(t_token **tokens_list)
{
	t_token	*current_node;
	t_token	*next_node;

	if (!tokens_list)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	current_node = (t_token *) *tokens_list;
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
		if (current_node->expands_list)
			free_expands_list((*tokens_list)->expands_list);

		free(current_node);		// Liberar nodo actual		
		current_node = next_node; 
	}
	*tokens_list = NULL;
	ft_printf(FREE_TOKENS_LIST);
}

void	free_expands_list(t_expand *expands_list)
{
	t_expand	*current_node;
	t_expand	*next_node;

	if (!expands_list)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	current_node = (t_expand *) expands_list;
	while (current_node)
	{
		next_node = current_node->next;

		// Liberar strings
		if (expands_list->substitution_str)
			free(expands_list->substitution_str);
		if (expands_list->key)
			free(expands_list->key);
		if (expands_list->value)
			free(expands_list->value);

		free(current_node);		// Liberar nodo actual		
		current_node = next_node;
	}
	ft_printf(FREE_EXPANDS_LIST);
}

void	free_matrix(char **matrix)
{
	int row;

	if (!matrix)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);	
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