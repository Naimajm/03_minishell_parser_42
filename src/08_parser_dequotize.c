/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_parser_dequotize.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/09 20:27:42 by juagomez         ###   ########.fr       */
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
		printf("(dequotize) token->final_token-> %s\n", current_token->final_token);
		current_token = current_token->next;
	}
	
}

// funcion para quitar comillas de 'expanded_token' a 'final_token'
void	remove_quotes(t_token *token)
{
	int		index_expanded;
	char 	quote;
	int		first_index;

	if (!token)
		return ;
	index_expanded 	= 0;
	first_index		= 0;
	while (token->expanded_token[index_expanded])
	{
		if (is_quote(token->expanded_token[index_expanded]))
		{			
			quote = token->expanded_token[index_expanded]; // definir inicio comilla doble o simple
			index_expanded++;
			first_index = index_expanded;			
			while (token->expanded_token[index_expanded] &&
					token->expanded_token[index_expanded] != quote)  // calcular indice string entre comilla inicial y final
				index_expanded++;			
			token->final_token = ft_substr(token->expanded_token, first_index, index_expanded - first_index); // copiar texto sin comillas			
			if (token->expanded_token[index_expanded] == quote) // verificar para cierre final comilla
				index_expanded++;			
		}
		else
			token->final_token = ft_substr(token->expanded_token, 0, ft_strlen(token->expanded_token));	
	}
	//printf("(remove_quotes) token->final_token-> %s\n",token->final_token);
}
