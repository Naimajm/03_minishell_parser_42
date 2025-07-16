/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_lexical.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/16 21:33:23 by juagomez         ###   ########.fr       */
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
		
        if (!shell->input[index]) // Verificar si llegamos al final después de saltar espacios
            break;

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
    char	current_quote;
    bool	quote_state;  			// false = fuera, true = dentro
	char	character;

    if (!shell->input)
        return (FAILURE);    
    index = index_first_char;    
    current_quote = 0;
	quote_state = false;
    
    while (shell->input[index])
    {
        character = shell->input[index];
        
		// caso DENTRO DE COMILLAS -> ignorar espacios y operadores
        if (is_quote(character))
        {
            if (quote_state == 0) // Entrando en comillas
            {                
                current_quote = character;
                quote_state = true;
                index++;
            }
            else if (character == current_quote) // Saliendo de comillas del mismo tipo
            {                
                quote_state = false;
                current_quote = 0;
                index++;
            }
            else // Comilla diferente dentro de comillas actuales                     
                index++; // Se trata como carácter literal
        }

		// caso FUERA DE COMILLAS -> espacios y operadores terminan la palabra
        else if (quote_state == false && (is_space(character) || is_operator(character)))            
            break;	
		// Carácter normal o carácter dentro de comillas   	
        else             
            index++;      
    }
    
    token_input = ft_substr(shell->input, index_first_char, (index - index_first_char));
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
