/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/22 12:47:45 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	word_tokenizer(t_shell *shell, int index_first_char);
int	quotes_tokenizer(t_shell *shell, int index_first_char);
int	operator_tokenizer(t_shell *shell, int index_first_char);

void	tokenizer(t_shell *shell)
{
	int		index;
	int		token_len;

	if (!shell || !shell->input)
		return ; 	
	index = 0;	
	while (shell->input[index])
	{
		while (is_space(shell->input[index])) // ignorar espacios iniciales 
			index++;
		if (is_quote(shell->input[index])) // '\'' -> LITERAL // '\"' -> EXPANSION VARIABLE
			token_len = quotes_tokenizer(shell, index);
		else if (is_operator(shell->input[index]))
			token_len = operator_tokenizer(shell, index);		
		else // 1º letra palabra simple sin inicio comillas
			token_len = word_tokenizer(shell, index);	
		if (token_len == FAILURE)	// error
			return ;
		if (token_len == 0)	 // caso especial input = 0;
			index++;
		else
			index += token_len;				
	}		
}

int	word_tokenizer(t_shell *shell, int index_first_char)
{
	char	*token_input;
	int		index;
	int		len_input;

	if (!shell->input)
		return (FAILURE);
	index = index_first_char;
	len_input = 0;	
	// longitud de caracteres de la palabra -> limites > < | " " '"' /0
	while (!is_space(shell->input[index]) && !is_operator(shell->input[index]) 
		&& shell->input[index] !=  '\"' && shell->input[index])
		index++;	
	token_input = ft_substr(shell->input, index_first_char, (index - index_first_char)); // copiar sub substr
	if (!token_input)
		return (FAILURE);
	len_input = ft_strlen(token_input);
	add_token_node(&shell->token_list, token_input, NO_QUOTES);			
	free(token_input);
	//ft_printf("token -> %s\n", token);
	return (len_input);
}

int	quotes_tokenizer(t_shell *shell, int index_first_char)
{
	char	*token_input;
	char	delimiter;	
	int		current_index;	
	int		token_type;
	int		len_input;

	delimiter		= shell->input[index_first_char];
	current_index 	= index_first_char + 1;  // quitar comilla
	len_input 		= 0;	
	while (shell->input[current_index] && shell->input[current_index] != delimiter)
		current_index++;
	if (shell->input[current_index] != delimiter) // validacion falta de comilla de cierre
	{
		ft_printf(ERROR_QUOTE_SYNTAX);
		return (FAILURE);
	}	
	token_input = ft_substr(shell->input, index_first_char, (current_index - index_first_char + 1)); // copia expresion con comillas incluidas
	if (!token_input)
		return (FAILURE);	
	if (delimiter == '\"')
		token_type = DOUBLE_QUOTES;
	else if (delimiter == '\'')
		token_type = SINGLE_QUOTES;
	len_input =  ft_strlen(token_input);
	add_token_node(&shell->token_list, token_input, token_type);
	free(token_input);
	return (len_input);
}

int	operator_tokenizer(t_shell *shell, int index_first_char)
{
	int		len_input;

	if (shell->input[index_first_char] == '>') // operadores especiales -> OUTFILE o APPEND
	{
		if (shell->input[index_first_char + 1] == '>')
		{
			add_token_node(&shell->token_list, ">>", APPEND);
			return (len_input = 2);
		}				
		else
			add_token_node(&shell->token_list, ">", OUTFILE);		
	}		
	else if (shell->input[index_first_char] == '<') // operadores especiales -> INFILE o HERE_DOC
	{
		if (shell->input[index_first_char + 1] == '<')
		{
			add_token_node(&shell->token_list, "<<", HERE_DOC);
			return (len_input = 2);
		}			
		else
			add_token_node(&shell->token_list, "<", INFILE);							
	}		
	else if (shell->input[index_first_char] == '|') // operadores especiales -> PIPE
		add_token_node(&shell->token_list, "|", PIPE);	
	return (len_input = 1);
}

/* void	tokenizer_null(t_shell *shell)
{
	int		index;
	char	*token_input;

	if (!shell || !shell->input)
		return ; 	
	index = 0;	
	while (shell->input[index])
	{
		token_input		= NULL;	
		while (is_space(shell->input[index])) // ignorar espacios iniciales 
			index++;		
			
			
		if (shell->input[index] == '\'') // PALABRAS COMILLA SIMPLES -> LITERAL
		{
			token_input = quotes_tokenizer(shell->input, index, '\'');
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
			token_input = word_tokenizer(shell->input, index);	
			if (!token_input)
				return ;			
			index += ft_strlen(token_input) - 1;	 // avanza indice hasta final palabra	
			add_token_node(&shell->token_list, token_input, NO_QUOTES);			
			free(token_input);		 // liberar copia strdup de input	
		}			
		index++;		
	}	
} */

/* char	*quotes_tokenizer_null(char *input, int index_first_char, char delimiter)
{
	char	*token;
	int		index;
	int		second_quote_flag;		// flag validacion sintaxis comillas simples

	token	= NULL;
	index 	= index_first_char + 1;
	second_quote_flag = 0;		
	while (input[index]) // expresion literal -> no interpretacion especial operadores
	{		
		if (input[index] == delimiter) // condicion final expresion literal
		{			
			second_quote_flag = 1; // flag 2º comilla -> ok
			break;		
		}
		index++;
	}	
	if (!second_quote_flag) // validacion sintaxis expresion literal
	{
		ft_printf(ERROR_QUOTE_SYNTAX);
		return (NULL);
	}	
	token = ft_substr(input, index_first_char, (index - index_first_char + 1)); // copia expresion con comillas simples incluidas
	if (!token)
		return (NULL); //ft_printf("quote_token -> %s\n", token);		
	return (token);
} */

/* char	*word_tokenizer_null(char *input, int index_first_char)
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
} */
