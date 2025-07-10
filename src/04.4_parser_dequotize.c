/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04.4_parser_dequotize.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/10 12:59:49 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES PARA QUITAR COMILLAS + UNIR TOKENS

void	dequotize(t_token *token_list);
void	remove_quotes(t_token *token);


void	join_tokens(t_token *token_list)
{
	t_token	*current_token;

	if (!token_list)
		return ;
	current_token = (t_token *) token_list;
	while (current_token)
	{
		
		current_token = current_token->next;
	}
	



}

void	dequotize(t_token *token_list)
{
	t_token	*current_token;

	if (!token_list)
		return ;
	current_token = (t_token *) token_list;
	while (current_token)
	{
		// quitar comillas (current_token->type == SINGLE_QUOTES NO_QUOTES ?? poner en condicional)
		if (current_token->type == DOUBLE_QUOTES 
			|| current_token->type == SINGLE_QUOTES)
		{
			remove_quotes(current_token);
		}
		else
			current_token->noquotes_token = ft_strdup(current_token->expanded_token);	
		//printf("(dequotize) token->final_token-> %s\n", current_token->final_token);
		current_token = current_token->next;
	}
	}

// funcion para quitar comillas de 'expanded_token' a 'final_token'
void	remove_quotes(t_token *token)
{
	if (!token || !token->expanded_token)
		return ;		
	token->noquotes_token = ft_substr(token->expanded_token, 1, ft_strlen(token->expanded_token) - 2); 
}

