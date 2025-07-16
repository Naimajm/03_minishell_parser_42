/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_parser_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:14:13 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/16 21:21:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/// UNIR TODOS LOS 'NOQUOTES_TOKEN' de los nodos Token EN 'processed_word' word

void	insert_noquotes_token_node_value(t_word_token *word);

// INSERTAR VALORE EN TOKEN -> FINAL TOKEN
void	generate_processed_word(t_word_token **words_list)
{
	t_word_token	*current_word;
	
	if (!words_list)
		return ;	
	current_word = (t_word_token *) *words_list;	
	while (current_word)
	{
		printf("(generate_processed_word) word->raw_word 	-> %s\n", current_word->raw_word);

		insert_noquotes_token_node_value(current_word);

		printf("(generate_processed_word) word->processed_word 	-> %s\n\n", current_word->processed_word);
		current_word = current_word->next;
	}	
}
 
void	insert_noquotes_token_node_value(t_word_token *word)
{
	t_token	*current_token;
	char	*result;
	char	*temp;

	if (!word || !word->tokens_list)
		return ;
	
	if (word->processed_word) // Liberar processed_word anterior si existe
    {
        free(word->processed_word);
        word->processed_word = NULL;
    }
	result = ft_strdup(""); // Inicializar resultado con string vacío
	if (!result)
		return ;	
	current_token = (t_token *) word->tokens_list;

	while (current_token) // Recorrer todos los tokens de la palabra
	{		
		if (current_token->noquotes_token) // Solo concatenar si noquotes_token no es NULL
		{
			temp = ft_strjoin_free( result, current_token->noquotes_token );
			if (!temp)
				return ;
			result = temp;			
		}
		current_token = current_token->next;
	}		
	
	word->processed_word = result; // Asignar el resultado final a processed_word
}