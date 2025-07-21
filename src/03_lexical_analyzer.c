/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_lexical_analyzer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/21 23:03:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	command_extractor(t_cmd *command);
int		word_extractor(t_cmd *process_list, int start_index);
int		operator_extractor(t_cmd *process_list, int start_index);

void	lexical_analyzer(t_cmd *commands_list)
{
	t_cmd	*current_command;
	
	if (!commands_list)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR); 
		
	current_command = (t_cmd *) commands_list;
	while (current_command)
	{
		command_extractor(current_command);
		current_command = current_command->next;
	}	
}

void	command_extractor(t_cmd *command)
{
	char	*command_input;
	int		index;
	int		command_len;

	if (!command)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR); 
		
	command_input = command->command;
	index = 0;	
	
	while (command_input[index])
	{
		while (is_space(command_input[index])) // ignorar espacios iniciales 
			index++;		
		if (!command_input[index]) // Verificar si llegamos al final después de saltar espacios
			break;

		// CLASIFICACION WORD // OPERATOR
		if (is_operator(command_input[index]))
			command_len = operator_extractor(command, index);  
		else
			command_len = word_extractor(command, index);
			
		// GESTION CASOS ESPECIALES AVANCE INDEX
		index = advance_index_by_length(index, command_len);				
	}		
}

int	word_extractor(t_cmd *command, int start_index)
{
	char	*command_input;
	int		index;
	int		len_input;
	char	current_quote;
	bool	quote_state;  			// false = fuera, true = dentro
	char	character;

	if (!command->command)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);   
	index = start_index;    
	current_quote = 0;
	quote_state = false;
	
	while (command->command[index])
	{
		character = command->command[index];
	  
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
	
	command_input = ft_substr(command->command, start_index, (index - start_index));
	if (!command_input)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	
	len_input = ft_strlen(command_input);
	add_word_node(&command->words_list, command_input, WORD);
	free(command_input);
	
	return (len_input);
}

int	operator_extractor(t_cmd *command, int start_index)
{
	int		len_input;

	if (command->command[start_index] == '>') // operadores especiales -> OUTFILE o APPEND
	{
		if (command->command[start_index + 1] == '>')
		{
			add_word_node(&command->words_list, ">>", APPEND);
			return (len_input = 2);
		}				
		else
			add_word_node(&command->words_list, ">", OUTFILE);		
	}		
	else if (command->command[start_index] == '<') // operadores especiales -> INFILE o HERE_DOC
	{
		if (command->command[start_index + 1] == '<')
		{
			add_word_node(&command->words_list, "<<", HERE_DOC);
			return (len_input = 2);
		}			
		else
			add_word_node(&command->words_list, "<", INFILE);							
	}		
	else if (command->command[start_index] == '|') // operadores especiales -> PIPE
		add_word_node(&command->words_list, "|", PIPE);	
	return (len_input = 1);
}
