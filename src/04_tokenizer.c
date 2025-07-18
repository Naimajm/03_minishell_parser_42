/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 12:33:06 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	noquotes_tokenizer(t_word *word, int index_first_char);
int	quotes_tokenizer(t_word *word, int index_first_char);
int	operator_tokenizer(t_word *word, int index_first_char);

void	tokenizer(t_word *words_list)
{
	t_word	*current_word;
	char	*raw_word;
	int		index;
	int		token_len;

	if (!words_list)
		return ; 	
	current_word = (t_word *) words_list;		

	while (current_word)
	{
		index = 0;
		raw_word = current_word->raw_word;
		while (raw_word[index])
		{
			while (is_space(raw_word[index])) 			// ignorar espacios iniciales 
				index++;
			if (is_quote(raw_word[index])) 				// '\'' -> LITERAL // '\"' -> EXPANSION VARIABLE
				token_len = quotes_tokenizer(current_word, index);
			else if (is_operator(raw_word[index]))
				token_len = operator_tokenizer(current_word, index);		
			else 											// 1º letra palabra simple sin inicio comillas
				token_len = noquotes_tokenizer(current_word, index);	
			if (token_len == FAILURE)						// error
				return ;
			if (token_len == 0)	 							// caso especial input = 0;
				index++;
			else
				index += token_len;				
		}
		current_word = current_word->next;
	}			
}

int	noquotes_tokenizer(t_word *word, int index_first_char)
{
	char	*raw_word;
	char	*token_input;
	int		index;
	int		len_input;

	if (!word)
		return (FAILURE);
	raw_word 	= (char *) word->raw_word;
	index 		= index_first_char;
	len_input 	= 0;	
	
	while (!is_space(raw_word[index]) // longitud de caracteres de la palabra -> limites > < | " " '"' /0
		&& !is_operator(raw_word[index]) 
		//&& raw_word[index] !=  '\"' 
		&& !is_quote(raw_word[index]) 
		&& raw_word[index])
		index++;	
		
	token_input = ft_substr(raw_word, index_first_char, (index - index_first_char)); // copiar sub substr
	if (!token_input)
		return (FAILURE);
	len_input = ft_strlen(token_input);
	add_token_node(&word->tokens_list, token_input, NO_QUOTES);			
	free(token_input);
	//ft_printf("token -> %s\n", token);
	return (len_input);
}

int	quotes_tokenizer(t_word *word, int index_first_char)
{
	char	*raw_word;
	char	*token_input;
	char	delimiter;	
	int		index;	
	int		token_type;
	int		len_input;

	raw_word 		= (char *) word->raw_word;
	delimiter		= raw_word[index_first_char];	// Comilla de apertura
	index 			= index_first_char + 1;  		// Empezar después de la comilla
	token_type 		= 0;
	len_input 		= 0;

	// Buscar comilla de cierre
	while (raw_word[index] && raw_word[index] != delimiter)
		index++;

	if (raw_word[index] == delimiter)
		index++;
	else					// validacion falta de comilla de cierre
		print_message_and_exit(ERROR_QUOTE_SYNTAX, STDERR_FILENO, FAILURE);


	token_input = ft_substr(raw_word, index_first_char, (index - index_first_char)); // copia expresion con comillas incluidas
	if (!token_input)
		return (FAILURE);	

	// Determinar tipo de token
	if (delimiter == '\"')
		token_type = DOUBLE_QUOTES;
	else if (delimiter == '\'')
		token_type = SINGLE_QUOTES;
	else
		token_type = NO_QUOTES;

	len_input =  ft_strlen(token_input);
	add_token_node(&word->tokens_list, token_input, token_type);
	free(token_input);
	return (len_input);
}

int	operator_tokenizer(t_word *word, int index_first_char)
{
	char	*raw_word;
	int		len_input;

	raw_word = (char *) word->raw_word;

	if (raw_word[index_first_char] == '>') 			// operadores especiales -> OUTFILE o APPEND
	{
		if (raw_word[index_first_char + 1] == '>')
		{
			add_token_node(&word->tokens_list, ">>", OPERATOR);
			return (len_input = 2);
		}				
		else
			add_token_node(&word->tokens_list, ">", OPERATOR);		
	}		
	else if (raw_word[index_first_char] == '<') 	// operadores especiales -> INFILE o HERE_DOC
	{
		if (raw_word[index_first_char + 1] == '<')
		{
			add_token_node(&word->tokens_list, "<<", OPERATOR);
			return (len_input = 2);
		}			
		else
			add_token_node(&word->tokens_list, "<", OPERATOR);							
	}		
	else if (raw_word[index_first_char] == '|') 	// operadores especiales -> PIPE
		add_token_node(&word->tokens_list, "|", OPERATOR);	
	return (len_input = 1);
}

/* int	operator_tokenizer(t_word_token *word, int index_first_char)
{
	char	*raw_word;
	int		len_input;
	char	*operator;

	raw_word = (char *) word->raw_word;


	if (raw_word[index_first_char] == '>') 			// operadores especiales -> OUTFILE o APPEND
	{
		if (raw_word[index_first_char + 1] == '>')
		{
			operator = ">";
			add_token_node(&word->tokens_list, ">>", OPERATOR);
			return (len_input = 2);
		}				
		else
			add_token_node(&word->tokens_list, ">", OPERATOR);		
	}		
	else if (raw_word[index_first_char] == '<') 	// operadores especiales -> INFILE o HERE_DOC
	{
		if (raw_word[index_first_char + 1] == '<')
		{
			add_token_node(&word->tokens_list, "<<", OPERATOR);
			return (len_input = 2);
		}			
		else
			add_token_node(&word->tokens_list, "<", OPERATOR);							
	}		
	else if (raw_word[index_first_char] == '|') 	// operadores especiales -> PIPE
		add_token_node(&word->tokens_list, "|", OPERATOR);	
	return (len_input = 1);
} */

