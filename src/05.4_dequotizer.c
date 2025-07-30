/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.4_dequotizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/30 17:31:03 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES PARA QUITAR COMILLAS + UNIR TOKENS
void	remove_quotes(t_token *token);

void	dequotize_tokens(t_word *words_list)
{
	t_word	*current_word;

	if (!words_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
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
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_token = (t_token *) tokens_list;
	while (current_token)
	{
		if (current_token->type == DOUBLE_QUOTES 
			|| current_token->type == SINGLE_QUOTES)
		{
			current_token->noquotes_token = ft_substr(current_token->expanded_token, 1, ft_strlen(current_token->expanded_token) - 2); 
			if (!current_token->noquotes_token)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
		}
		else
		{
			current_token->noquotes_token = ft_strdup(current_token->expanded_token);	
			if (!current_token->noquotes_token)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
		}	
		current_token = current_token->next;
	}	
}

