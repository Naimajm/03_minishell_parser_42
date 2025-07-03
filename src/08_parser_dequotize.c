/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_parser_dequotize.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/06/11 22:15:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES PARA QUITAR COMILLAS + UNIR TOKENS

void	remove_quotes(t_token *token);

void	dequotize(t_token *token_list)
{
	t_token	*current_token;
	//char	*new_token;

	if (!token_list)
		return ;

	current_token = (t_token *) token_list;
	while (current_token)
	{
		// quitar comillas
		if (current_token->type == SINGLE_QUOTES 
			|| current_token->type == DOUBLE_QUOTES 
			|| current_token->type == NO_QUOTES)
		{
			remove_quotes(current_token);
		}
		printf("dequotize / final->token -> %s", current_token->final_token);
		current_token = current_token->next;
	}
	
}

void	remove_quotes(t_token *token)
{
	int		index_expanded;
	int 	index_final;
	char 	quote;

	if (!token)
		return ;
	index_expanded 	= 0;
	index_final		= 0;
	while (token->expanded_token[index_expanded])
	{
		if (is_quote(token->expanded_token[index_expanded]))
		{
			// definir inicio comilla doble o simple
			quote = token->expanded_token[index_expanded];
			index_expanded++;
			// copiar texto sin comillas
			while (token->expanded_token[index_expanded] &&
					token->expanded_token[index_expanded] != quote)
			{
				token->final_token[index_final] = token->expanded_token[index_expanded];
				printf("token->final_token[index_final]-> %c",token->final_token[index_final]);
				index_expanded++;			
			}
			// verificar para cierre final comilla
			if (token->expanded_token[index_expanded] == quote)
				index_expanded++;			
		}
		else
		{
			token->final_token[index_final] = token->expanded_token[index_expanded];
			index_expanded++;
			index_final++;			
		}			
	}
	token->final_token[index_final] = '\0'; // terminador nulo	
}

// funcion para quitar comillas de 'expanded_token' a 'final_token'
/* void	remove_quotes(t_token *token)
{
	int		index_expanded;
	int 	index_final;
	char 	quote;

	if (!token)
		return ;
	index_expanded 	= 0;
	index_final		= 0;
	while (token->expanded_token[index_expanded])
	{
		if (is_quote(token->expanded_token[index_expanded]))
		{
			// definir comilla doble o simple
			quote = token->expanded_token[index_expanded];
			index_expanded++;
			// copiar texto sin comillas
			while (token->expanded_token[index_expanded] &&
					token->expanded_token[index_expanded] != quote)
			{
				token->final_token[index_final] = token->expanded_token[index_expanded];
				printf("token->final_token[index_final]-> %c",token->final_token[index_final]);
				index_expanded++;
				index_final++;				
			}
			// verificar para cierre final comilla
			if (token->expanded_token[index_expanded] == quote)
				index_expanded++;			
		}
		else
		{
			token->final_token[index_final] = token->expanded_token[index_expanded];
			index_expanded++;
			index_final++;			
		}			
	}
	token->final_token[index_final] = '\0'; // terminador nulo	
} */
