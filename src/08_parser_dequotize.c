/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_parser_dequotize.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/09 21:17:17 by juagomez         ###   ########.fr       */
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
		else
			current_token->final_token = ft_strdup(current_token->expanded_token);	
		//printf("(dequotize) token->final_token-> %s\n", current_token->final_token);
		current_token = current_token->next;
	}
	}

// funcion para quitar comillas de 'expanded_token' a 'final_token'
void	remove_quotes(t_token *token)
{
	int		index;
	char 	quote;
	int		first_index;

	if (!token || !token->expanded_token)
		return ;
	index 			= 0;
	first_index		= 0;
	while (token->expanded_token[index])
	{
		if (is_quote(token->expanded_token[index]))
		{			
			quote = token->expanded_token[index]; // definir inicio comilla doble o simple
			index++;
			first_index = index;			
			while (token->expanded_token[index] &&
					token->expanded_token[index] != quote)  // calcular indice string entre comilla inicial y final
				index++;			
			token->final_token = ft_substr(token->expanded_token, first_index, index - first_index); // copiar texto sin comillas			
			if (token->expanded_token[index] == quote) // verificar para cierre final comilla
				index++;			
		}
		else
		{
			token->final_token = ft_strdup(token->expanded_token);	
			index++;
		}			
	}
	//printf("(remove_quotes) token->final_token-> %s\n",token->final_token);
}
