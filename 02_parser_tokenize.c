/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/05 19:01:52 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static int	is_space(char character);
t_token	*initialize_token();



void	categorize_token(t_shell *shell)
{
	int	index;
	int	type_token;

	if (!shell)
		return ; 
	
	index 		= 0;
	type_token 	= 0;
	
	// CATEGORIZACION TOKENs -> 7 TIPOS TOKENS
	while (shell->input[index])
	{
		// ignorar espacios iniciales 
		while (is_space(shell->input[index]))
			index++;	
		
		// PALABRAS COMILLA SIMPLES -> LITERAL
		if (shell->input[index] == '\'')
		{
			ft_printf("inicio palabra comillas simples -> CONTENIDO LITERAL\n");
			type_token = WORDS_SINGLE_QUOTES;
		}		
		// PALABRAS COMILLA DOBLE -> EXPANSION VARIABLE
		else if (shell->input[index] == '"')
		{
			ft_printf("inicio palabra comillas dobles -> EXPANSION VARIABLES\n");
			type_token = WORDS_DOUBLE_QUOTES;
		}
		// operadores especiales -> OUTFILE o APPEND
		else if (shell->input[index] == '>')
		{
			if (shell->input[index + 1] == '>')
			{
				index++;
				ft_printf("APPEND\n");
				type_token = APPEND;
			}
			else
			{
				ft_printf("OUTFILE \n");
				type_token = OUTFILE;
			}				
		}	
		// operadores especiales -> INFILE o HERE_DOC
		else if (shell->input[index] == '<')
		{
			if (shell->input[index + 1] == '<')
			{
				index++;
				ft_printf("HERE_DOC\n");
				type_token = HERE_DOC;
			}
			else
			{
				ft_printf("INFILE \n");
				type_token = INFILE;
			}				
		}			
		// operadores especiales -> PIPE
		else if (shell->input[index] == '|')
		{
			ft_printf("operador PIPE\n");
			type_token = PIPE;
		}			
		else // 1º letra palabra simple
		{
			ft_printf("%c -> \n", shell->input[index]);
			type_token = WORDS_NO_QUOTATION;			
		}			
		index++;
	}	
}

t_token	*initialize_token()
{
	t_token	*token;

	token->token 		= NULL;
	token->type_token 	= 0;
	token->previous		= NULL;
	token->next			= NULL;	

	return (token);
}


static int	is_space(char character)
{
	int	space;

	space = 0;
	if (character == ' ' || (character >= '\t' && character <= '\r'))
		space = 1;
	return (space);
}