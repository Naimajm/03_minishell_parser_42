/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_word_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:14:13 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/25 11:24:05 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/// UNIR TODOS LOS 'NOQUOTES_TOKEN' de los nodos Token EN 'processed_word' word

void	insert_token_node(t_word *word);

// INSERTAR VALORE EN TOKEN -> FINAL TOKEN
void	generate_processed_word(t_word **words_list)
{
	t_word	*current_word;
	
	if (!words_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));	
	current_word = (t_word *) *words_list;	
	while (current_word)
	{
		printf("(generate_processed_word) word->raw_word 	-> %s\n", current_word->raw_word);

		insert_token_node(current_word);

		printf("(generate_processed_word) word->processed_word 	-> %s\n\n", current_word->processed_word);
		current_word = current_word->next;
	}	
}
 
void	insert_token_node(t_word *word)
{
	t_token	*current_token;
	char	*result;
	char	*temp;

	if (!word || !word->tokens_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	
	if (word->processed_word) // Liberar processed_word anterior si existe
	{
		free(word->processed_word);
		word->processed_word = NULL;
	}
	result = ft_strdup(""); // Inicializar resultado con string vacío
	if (!result)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));	
	current_token = (t_token *) word->tokens_list;

	while (current_token) // Recorrer todos los tokens de la palabra
	{		
		if (current_token->noquotes_token) // Solo concatenar si noquotes_token no es NULL
		{
			temp = ft_strjoin_free( result, current_token->noquotes_token );
			if (!temp)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));	
			result = temp;			
		}
		current_token = current_token->next;
	}			
	word->processed_word = result; // Asignar el resultado final a processed_word
}