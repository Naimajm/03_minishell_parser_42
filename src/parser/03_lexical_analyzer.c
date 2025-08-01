/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_lexical_analyzer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/01 17:19:06 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		command_extractor(t_cmd *command);
int		word_extractor(t_cmd *process_list, int start_index);
int		operator_extractor(t_cmd *process_list, int start_index);

void	lexical_analyzer(t_cmd *current_command, t_shell *shell)
{	
	if (!current_command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));				
	if (command_extractor(current_command) == FAILURE)
	{
		ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO);
        shell->exit_status = ERROR;
		return ;
	}
}

int	command_extractor(t_cmd *command)
{
	char	*command_input;
	int		index;
	int		word_len;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE); 	
	command_input = command->command;
	index = 0;		
	while (command_input[index])
	{
		while (is_space(command_input[index])) 	// ignorar espacios iniciales 
			index++;		
		if (!command_input[index]) 				// Verificar si llegamos al final después de saltar espacios
			break;		
		if (is_redirection(command_input[index]) || is_pipe(command_input[index]))  // CLASIFICACION WORD // OPERATOR
			word_len = operator_extractor(command, index);  
		else
			word_len = word_extractor(command, index);				
		if (word_len == FAILURE)
			return (FAILURE);			
		index = advance_index_by_length(index, word_len);		// GESTION CASOS ESPECIALES AVANCE INDEX	
	}	
	return (SUCCESS);		
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
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE); 		 
	index 			= start_index;    
	current_quote 	= 0;
	quote_state 	= false;	
	while (command->command[index])
	{
		character = command->command[index];  		
		if (is_quote(character))		// caso DENTRO DE COMILLAS -> ignorar espacios y operadores
		{
			if (quote_state == 0) 		// Entrando en comillas
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
			else 						// Comilla diferente dentro de comillas actuales                     
				index++; 				// Se trata como carácter literal
		}
		// caso FUERA DE COMILLAS -> espacios y operadores terminan la palabra
		else if (quote_state == false && (is_space(character) || is_redirection(character) || is_pipe(character)))  
			break;		 	
		else    			// Carácter normal o carácter dentro de comillas           
			index++;      
	}	
	command_input = ft_substr(command->command, start_index, (index - start_index));
	if (!command_input)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
	
	len_input = ft_strlen(command_input);

	//printf("Generated word: <%s>\n", command_input);		
	add_word_node(&command->words_list, command_input, WORD);
	//printf("DEBUG: add word successfully -> word_len = %d\n\n", len_input);
	
	free(command_input);	
	return (len_input);
}

int	operator_extractor(t_cmd *command, int start_index)
{
	char	*input;
	int		operator_len;	

	if (!command || start_index < 0)
		return (FAILURE);
	input = command->command;
	operator_len = get_operator_length(command->command, start_index);

	if (operator_len == 2 && input[start_index] == '>')		
        add_word_node(&command->words_list, ">>", APPEND);  
    else if (operator_len == 2 && input[start_index] == '<')	
        add_word_node(&command->words_list, "<<", HERE_DOC);
    else if (operator_len == 1 && input[start_index] == '>')
        add_word_node(&command->words_list, ">", OUTFILE);
    else if (operator_len == 1 && input[start_index] == '<')
        add_word_node(&command->words_list, "<", INFILE);
    else if (operator_len == 1 && is_pipe(input[start_index]))
        add_word_node(&command->words_list, "|", PIPE);
	else
		return (FAILURE); 
	return (operator_len);
}
