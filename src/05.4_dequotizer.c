/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.4_dequotizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/21 13:25:06 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES PARA QUITAR COMILLAS + UNIR TOKENS
void	remove_quotes(t_token *token);

void	dequotize_tokens(t_word *words_list)
{
	t_word	*current_word;

	if (!words_list)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);
	current_word = (t_word *) words_list;
	while (current_word)
	{
		remove_quotes(current_word->tokens_list);
		current_word = current_word->next;
	}
}

// funcion para quitar comillas de 'expanded_token' a 'final_token'
void	remove_quotes(t_token *tokens_list)
{
	t_token *current_token;

	if (!tokens_list)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);
	current_token = (t_token *) tokens_list;
	while (current_token)
	{
		// quitar comillas (current_token->type == SINGLE_QUOTES NO_QUOTES ?? poner en condicional)
		if (current_token->type == DOUBLE_QUOTES 
			|| current_token->type == SINGLE_QUOTES)
		{
			current_token->noquotes_token = ft_substr(current_token->expanded_token, 1, ft_strlen(current_token->expanded_token) - 2); 
			if (!current_token->noquotes_token)
				perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
		}
		else
			current_token->noquotes_token = ft_strdup(current_token->expanded_token);	
			if (!current_token->noquotes_token)
				perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
		//printf("(dequotize) token->final_token-> %s\n", current_token->final_token);		
		current_token = current_token->next;
	}	
}

