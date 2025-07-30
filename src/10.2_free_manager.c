/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.2_free_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:31:55 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/30 19:09:58 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_commands_list(t_cmd **commands_list);
void	free_words_list(t_word **words_list);
void	free_tokens_list(t_token **token_list);
void	free_expands_list(t_expand **expands_list);

void	free_iteration_input(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->commands_list)		// Liberar lista comandos iteracion
		free_commands_list(&shell->commands_list);
	printf(FREE_ITERATION_INPUT);
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
		
		free(current_node);
		current_node = next_node;
	}
	*commands_list = NULL;
	//printf(FREE_COMMANDS_LIST);	
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

		free(current_node);
		current_node = next_node;
	}    
	*words_list = NULL;
	//printf(FREE_WORDS_LIST);
}

void	free_tokens_list(t_token **tokens_list)
{
	t_token	*current_node;
	t_token	*next_node;

	if (!tokens_list)
		return ;
	current_node = (t_token *) *tokens_list;
	while (current_node)
	{
		next_node = current_node->next;
		
		// Liberar strings
		if (current_node->raw_token) 
			free(current_node->raw_token);		
		if (current_node->expanded_token)
			free(current_node->expanded_token);
		if (current_node->noquotes_token) 	
			free(current_node->noquotes_token);

		// Liberar lista nodos expand		
		if (current_node->expands_list)
			free_expands_list(&current_node->expands_list);

		free(current_node);		// Liberar nodo actual		
		current_node = next_node; 
	}
	*tokens_list = NULL;
	//printf(FREE_TOKENS_LIST);
}

void	free_expands_list(t_expand **expands_list)
{
	t_expand	*current_node;
	t_expand	*next_node;

	if (!expands_list || !*expands_list)
		return ;
	current_node = (t_expand *) *expands_list;
	while (current_node)
	{
		next_node = current_node->next;

		// Liberar strings
		if (current_node->substitution_str)
			free(current_node->substitution_str);
		if (current_node->key)
			free(current_node->key);
		if (current_node->value)
			free(current_node->value);

		free(current_node);
		current_node = next_node;
	}
	*expands_list = NULL;
	//printf(FREE_EXPANDS_LIST);
}

