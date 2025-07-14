/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/14 17:23:46 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	word_extractor(t_shell *shell, int index_first_char);
int	operator_extractor(t_shell *shell, int index_first_char);

void	lexical_analyzer(t_shell *shell)
{
	int		index;
	int		word_len;

	if (!shell || !shell->input)
		return ; 	
	index = 0;	
	while (shell->input[index])
	{
		while (is_space(shell->input[index])) // ignorar espacios iniciales 
			index++;

		// CLASIFICACION WORD // OPERATOR
		if (is_operator(shell->input[index]))
			word_len = operator_extractor(shell, index);
		else
			word_len = word_extractor(shell, index);
		if (word_len == FAILURE)	// error
			return ;
		if (word_len == 0)	 // caso especial input = 0;
			index++;
		else
			index += word_len;				
	}	
	//print_words_list(shell->words_list);
}

int	word_extractor(t_shell *shell, int index_first_char)
{
	char	*token_input;
	int		index;
	int		len_input;
	char	quote;

	if (!shell->input)
		return (FAILURE);
	index = index_first_char;
	len_input = 0;	

	// CASO TOKEN CON COMILLAS DENTRO DE PALABRA
	while (!is_space(shell->input[index]) && !is_operator(shell->input[index]) 
		&& shell->input[index]) // Buscar el final de la palabra considerando comillas
	{
		// Si encontramos una comilla, buscar su cierre
		if (is_quote(shell->input[index]))
		{
			quote = shell->input[index];
			index++;
			while (shell->input[index] && shell->input[index] != quote) // Buscar comilla de cierre
				index++;
			// Saltar comilla de cierre si existe
			if (shell->input[index] == quote)
				index++;
		}
		else
			index++;
	}
	token_input = ft_substr(shell->input, index_first_char, (index - index_first_char)); // copiar sub substr
	if (!token_input)
		return (FAILURE);
	len_input = ft_strlen(token_input);
	add_word_node(&shell->words_list, token_input, WORD);			
	free(token_input);	
	return (len_input);
}

int	operator_extractor(t_shell *shell, int index_first_char)
{
	int		len_input;

	if (shell->input[index_first_char] == '>') // operadores especiales -> OUTFILE o APPEND
	{
		if (shell->input[index_first_char + 1] == '>')
		{
			add_word_node(&shell->words_list, ">>", OPERATOR);
			return (len_input = 2);
		}				
		else
			add_word_node(&shell->words_list, ">", OPERATOR);		
	}		
	else if (shell->input[index_first_char] == '<') // operadores especiales -> INFILE o HERE_DOC
	{
		if (shell->input[index_first_char + 1] == '<')
		{
			add_word_node(&shell->words_list, "<<", OPERATOR);
			return (len_input = 2);
		}			
		else
			add_word_node(&shell->words_list, "<", OPERATOR);							
	}		
	else if (shell->input[index_first_char] == '|') // operadores especiales -> PIPE
		add_word_node(&shell->words_list, "|", OPERATOR);	
	return (len_input = 1);
}
