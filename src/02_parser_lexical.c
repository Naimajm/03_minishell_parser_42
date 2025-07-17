/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_parser_lexical.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/17 11:41:19 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	word_extractor(t_command *process_list, int index_first_char);
int	operator_extractor(t_command *process_list, int index_first_char);

void	lexical_analyzer(t_command *commands_list)
{
	t_command	*current_command;
	char		*chunck_input;
	int			index;
	int			chunk_len;

	if (!commands_list || !commands_list->chunk_input)
		return ; 
		
	current_command = (t_command *) commands_list;

	while (current_command)
	{
		index = 0;	
		chunck_input = current_command->chunk_input;
		while (chunck_input[index])
		{
			while (is_space(chunck_input[index])) // ignorar espacios iniciales 
				index++;		
			if (!chunck_input[index]) // Verificar si llegamos al final después de saltar espacios
				break;

			// CLASIFICACION WORD // OPERATOR
			if (is_operator(chunck_input[index]))
				chunk_len = operator_extractor(current_command, index);  
			else
				chunk_len = word_extractor(current_command, index);
			if (chunk_len == FAILURE)	// error
				return ;
			if (chunk_len == 0)	 // caso especial input = 0;
				index++;
			else
				index += chunk_len;				
		}	
		current_command = current_command->next;
	}	
}

int	word_extractor(t_command *command, int index_first_char)
{
    char	*chunck_input;
    int		index;
    int		len_input;
    char	current_quote;
    bool	quote_state;  			// false = fuera, true = dentro
	char	character;

    if (!command->chunk_input)
        return (FAILURE);    
    index = index_first_char;    
    current_quote = 0;
	quote_state = false;
    
    while (command->chunk_input[index])
    {
        character = command->chunk_input[index];
      
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
    
    chunck_input = ft_substr(command->chunk_input, index_first_char, (index - index_first_char));
    if (!chunck_input)
        return (FAILURE);
    
    len_input = ft_strlen(chunck_input);
    add_word_node(&command->words_list, chunck_input, WORD);
    free(chunck_input);
    
    return (len_input);
}

int	operator_extractor(t_command *commands_list, int index_first_char)
{
	int		len_input;

	if (commands_list->chunk_input[index_first_char] == '>') // operadores especiales -> OUTFILE o APPEND
	{
		if (commands_list->chunk_input[index_first_char + 1] == '>')
		{
			add_word_node(&commands_list->words_list, ">>", APPEND);
			return (len_input = 2);
		}				
		else
			add_word_node(&commands_list->words_list, ">", OUTFILE);		
	}		
	else if (commands_list->chunk_input[index_first_char] == '<') // operadores especiales -> INFILE o HERE_DOC
	{
		if (commands_list->chunk_input[index_first_char + 1] == '<')
		{
			add_word_node(&commands_list->words_list, "<<", HERE_DOC);
			return (len_input = 2);
		}			
		else
			add_word_node(&commands_list->words_list, "<", INFILE);							
	}		
	else if (commands_list->chunk_input[index_first_char] == '|') // operadores especiales -> PIPE
		add_word_node(&commands_list->words_list, "|", PIPE);	
	return (len_input = 1);
}
