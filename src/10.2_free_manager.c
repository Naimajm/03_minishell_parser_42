/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.2_free_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:55 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 13:21:10 by juagomez         ###   ########.fr       */
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
		return ;
	
	if (shell->environment)						// free copia variables entorno
		free_matrix(shell->environment);

	if (shell->command_list) 					// free lista comandos
		free_commands_list(&shell->command_list);

	free(shell);
	shell = NULL;
	ft_printf(FREE_ALL_SHELL);
}

void	free_commands_list(t_cmd **commands_list)
{
	t_cmd	*current_node;
	t_cmd	*next_node;

	if (!commands_list)
		return ;
	
	current_node = (t_cmd *) *commands_list;
	while (current_node)
	{
		next_node = current_node->next;

		free_matrix(current_node->args);

		// Liberar strings
		if (current_node->chunk_input)
			free(current_node->chunk_input);
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
		return ;

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
			free_expands_list((*token_list)->expand_list);

		free(current_node);		// Liberar nodo actual		
		current_node = next_node; 
	}
	*token_list = NULL;
	ft_printf(FREE_TOKENS);
}

void	free_expands_list(t_expand *expand_list)
{
	if (!expand_list)
		return ;

	if (expand_list->key)
		free(expand_list->key);
	if (expand_list->value)
		free(expand_list->value);
	free(expand_list);		// Liberar nodo actual
	expand_list = NULL;
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