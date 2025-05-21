/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/21 13:47:25 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	*quotes_tokenizer(char *input, int index_first_char, char delimiter);
char	*word_tokenizer(char *input, int index_first_char);

void	tokenizer(t_shell *shell)
{
	int		index;
	char	*token_input;

	if (!shell || !shell->input)
		return ; 	
	index = 0;	

	// CATEGORIZACION TOKENs -> 7 TIPOS TOKENS
	while (shell->input[index])
	{
		token_input		= NULL;	

		while (is_space(shell->input[index])) // ignorar espacios iniciales 
			index++;		
		
		if (shell->input[index] == '\'') // PALABRAS COMILLA SIMPLES -> LITERAL
		{
			token_input = quotes_tokenizer(shell->input, index, '\'');
			// validacion token si en funcion NULL o expresion literal sin final comilla
			if (!token_input)
				return ;
			index += ft_strlen(token_input) - 1; // avanza indice hasta final palabra

			add_token_node(&shell->token_list, token_input, SINGLE_QUOTES);
			free(token_input); // liberar copia strdup de input
		}		
		else if (shell->input[index] == '"') // PALABRAS COMILLA DOBLE -> EXPANSION VARIABLE
		{
			token_input = quotes_tokenizer(shell->input, index, '"');
			if (!token_input)
				return ;
			index += ft_strlen(token_input) - 1;

			add_token_node(&shell->token_list, token_input, DOUBLE_QUOTES);
			free(token_input);			
		}		
		else if (shell->input[index] == '>') // operadores especiales -> OUTFILE o APPEND
		{
			if (shell->input[index + 1] == '>')
			{
				index++;
				add_token_node(&shell->token_list, ">>", APPEND);				
			}
			else
				add_token_node(&shell->token_list, ">", OUTFILE);			
		}		
		else if (shell->input[index] == '<') // operadores especiales -> INFILE o HERE_DOC
		{
			if (shell->input[index + 1] == '<')
			{
				index++;
				add_token_node(&shell->token_list, "<<", HERE_DOC);
			}
			else
				add_token_node(&shell->token_list, "<", INFILE);					
		}		
		else if (shell->input[index] == '|') // operadores especiales -> PIPE
			add_token_node(&shell->token_list, "|", PIPE);
		else // 1º letra palabra simple sin inicio comillas
		{
			// PROCESO COMPLETO CATEGORIZAR -> TOKENIZAR -> AÑADIR A LISTA TOKEN
			token_input = word_tokenizer(shell->input, index);	
			if (!token_input)
				return ;			
			index += ft_strlen(token_input) - 1;	 // avanza indice hasta final palabra	

			// añadir token a lista token
			add_token_node(&shell->token_list, token_input, NO_QUOTES);			
			free(token_input);		 // liberar copia strdup de input	
		}			
		index++;		
	}	
}

char	*quotes_tokenizer(char *input, int index_first_char, char delimiter)
{
	char	*token;
	int		index;
	int		second_quote_flag;		// flag validacion sintaxis comillas simples

	if (!input)
		return (NULL);	
	token	= NULL;
	index 	= index_first_char + 1;
	second_quote_flag = 0;
	//ft_printf("1º comilla ? -> %c\n", input[index_first_char]);
		
	while (input[index]) // expresion literal -> no interpretacion especial operadores
	{		
		if (input[index] == delimiter) // condicion final expresion literal
		{			
			second_quote_flag = 1; // flag 2º comilla -> ok
			break;		
		}
		index++;
	}
	// validacion sintaxis expresion literal
	if (!second_quote_flag)
	{
		ft_printf(ERROR_QUOTE_SYNTAX);
		return (NULL);
	}
	// copia expresion con comillas simples incluidas
	token = ft_substr(input, index_first_char, (index - index_first_char + 1)); // + 1-> 2º comillas
	if (!token)
		return (NULL);
	//ft_printf("quote_token -> %s\n", token);	
	return (token);
}

char	*word_tokenizer(char *input, int index_first_char)
{
	char	*token;
	int		index;

	if (!input)
		return (NULL);

	index = index_first_char;	
	//ft_printf("str -> %s [%d]\n", input, index_first_char);

	// longitud de caracteres de la palabra -> limites > < | " " '"' /0
	while (!is_space(input[index]) && !is_operator(input[index]) 
		&& input[index] !=  '\"' && input[index])
		index++;
		
	//ft_printf("index -> %d\n", index);
	// copiar sub substr
	token = ft_substr(input, index_first_char, (index - index_first_char));
	if (!token)
		return (NULL);

	//ft_printf("token -> %s\n", token);
	return (token);
}
