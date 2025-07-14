/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_parser_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/14 22:45:47 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	noquotes_tokenizer(t_shell *shell, int index_first_char);
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
		while (is_space(shell->input[index])) 			// ignorar espacios iniciales 
			index++;
		if (is_quote(shell->input[index])) 				// '\'' -> LITERAL // '\"' -> EXPANSION VARIABLE
			token_len = quotes_tokenizer(shell, index);
		else if (is_operator(shell->input[index]))
			token_len = operator_tokenizer(shell, index);		
		else 											// 1º letra palabra simple sin inicio comillas
			token_len = noquotes_tokenizer(shell, index);	
		if (token_len == FAILURE)						// error
			return ;
		if (token_len == 0)	 							// caso especial input = 0;
			index++;
		else
			index += token_len;				
	}		
}

int	noquotes_tokenizer(t_shell *shell, int index_first_char)
{
	char	*token_input;
	int		index;
	int		len_input;

	if (!shell->input)
		return (FAILURE);
	index = index_first_char;
	len_input = 0;	
	
	while (!is_space(shell->input[index]) // longitud de caracteres de la palabra -> limites > < | " " '"' /0
		&& !is_operator(shell->input[index]) 
		&& shell->input[index] !=  '\"' 
		&& shell->input[index])
		index++;	
	token_input = ft_substr(shell->input, index_first_char, (index - index_first_char)); // copiar sub substr
	if (!token_input)
		return (FAILURE);
	len_input = ft_strlen(token_input);
	add_token_node(&shell->words_list->tokens_list, token_input, NO_QUOTES);			
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
	token_type 		= 0;
	len_input 		= 0;	
	while (shell->input[current_index] && shell->input[current_index] != delimiter)
		current_index++;
	if (shell->input[current_index] != delimiter) // validacion falta de comilla de cierre
		print_message_and_exit(ERROR_QUOTE_SYNTAX, STDERR_FILENO, FAILURE);
	token_input = ft_substr(shell->input, index_first_char, (current_index - index_first_char + 1)); // copia expresion con comillas incluidas
	if (!token_input)
		return (FAILURE);	
	if (delimiter == '\"')
		token_type = DOUBLE_QUOTES;
	else if (delimiter == '\'')
		token_type = SINGLE_QUOTES;
	len_input =  ft_strlen(token_input);
	add_token_node(&shell->words_list->tokens_list, token_input, token_type);
	free(token_input);
	return (len_input);
}

int	operator_tokenizer(t_shell *shell, int index_first_char)
{
	int		len_input;

	if (shell->input[index_first_char] == '>') 			// operadores especiales -> OUTFILE o APPEND
	{
		if (shell->input[index_first_char + 1] == '>')
		{
			add_token_node(&shell->words_list->tokens_list, ">>", APPEND);
			return (len_input = 2);
		}				
		else
			add_token_node(&shell->words_list->tokens_list, ">", OUTFILE);		
	}		
	else if (shell->input[index_first_char] == '<') 	// operadores especiales -> INFILE o HERE_DOC
	{
		if (shell->input[index_first_char + 1] == '<')
		{
			add_token_node(&shell->words_list->tokens_list, "<<", HERE_DOC);
			return (len_input = 2);
		}			
		else
			add_token_node(&shell->words_list->tokens_list, "<", INFILE);							
	}		
	else if (shell->input[index_first_char] == '|') 	// operadores especiales -> PIPE
		add_token_node(&shell->words_list->tokens_list, "|", PIPE);	
	return (len_input = 1);
}

