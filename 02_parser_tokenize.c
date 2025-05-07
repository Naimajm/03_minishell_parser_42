/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/07 14:59:55 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	*word_tokenizer(char *input, int index_first_char);
static int	is_space(char character);

void	categorize_token(t_shell *shell)
{
	int		index;
	int		token_type;
	char	*token_input;

	if (!shell)
		return ; 
	
	index 		= 0;
	token_type 	= 0;
	token_input		= NULL;
	
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
			token_type = WORDS_SINGLE_QUOTES;
		}		
		// PALABRAS COMILLA DOBLE -> EXPANSION VARIABLE
		else if (shell->input[index] == '"')
		{
			ft_printf("inicio palabra comillas dobles -> EXPANSION VARIABLES\n");
			token_type = WORDS_DOUBLE_QUOTES;
		}
		// operadores especiales -> OUTFILE o APPEND
		else if (shell->input[index] == '>')
		{
			if (shell->input[index + 1] == '>')
			{
				index++;
				ft_printf("APPEND\n");
				token_type = APPEND;
			}
			else
			{
				ft_printf("OUTFILE \n");
				token_type = OUTFILE;
			}				
		}	
		// operadores especiales -> INFILE o HERE_DOC
		else if (shell->input[index] == '<')
		{
			if (shell->input[index + 1] == '<')
			{
				index++;
				ft_printf("HERE_DOC\n");
				token_type = HERE_DOC;
			}
			else
			{
				ft_printf("INFILE \n");
				token_type = INFILE;
			}				
		}			
		// operadores especiales -> PIPE
		else if (shell->input[index] == '|')
		{
			ft_printf("operador PIPE\n");
			token_type = PIPE;
		}			
		else // 1º letra palabra simple
		{			
			ft_printf("%c [%d]-> \n", shell->input[index],index);
			
			token_type = WORDS_NO_QUOTATION;
			token_input = word_tokenizer(shell->input, index);				
			//ft_printf("token -> %s\n", token_input);

			// avanza indice hasta final palabra
			index = index + ft_strlen(token_input) - 1;	
			ft_printf("%c [%d] \n", shell->input[index], index);

			// añadir token a lista token
			listtoken_add_back(&shell->token_list, token_input, token_type);
			
			//ft_printf("%shell->token -> [%d]\n", shell->token_list->token, shell->token_list->type);
			//ft_printf("%shell->token -> \n", shell->token_list->token);
			// verificacion
			print_token_list(shell->token_list);
		}			
		index++;
	}	
}

char	*word_tokenizer(char *input, int index_first_char)
{
	char	*token;
	int		index;

	if (!input)
		return (NULL);

	index = index_first_char;	
	//ft_printf("str -> %s [%d]\n", input, index_first_char);

	// longitud de caracteres hasta nuevo espacio
	while (!is_space(input[index]) && input[index])
		index++;
		
	//ft_printf("index -> %d\n", index);
	// copiar sub substr
	token = ft_substr(input, index_first_char, (index - index_first_char));

	//ft_printf("token -> %s\n", token);
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