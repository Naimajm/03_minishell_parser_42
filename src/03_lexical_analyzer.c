/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_lexical_analyzer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/27 12:08:50 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	command_extractor(t_cmd *command);
int		word_extractor(t_cmd *process_list, int start_index);
int		operator_extractor(t_cmd *process_list, int start_index);

void	lexical_analyzer(t_cmd *commands_list)
{
	t_cmd	*current_command;
	
	printf("DEBUG -> lexical_analyzer()\n");
	if (!commands_list)
	{					
		ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO);
		//return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO)); 
		return ;
	}		
		
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
	int		word_len;

	if (!command)
	{
		//printf("DEBUG -> lexical_analyzer()\n");			
		ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO);
		//return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO)); 
		return ;
	}		
	command_input = command->command;
	index = 0;	
	
	while (command_input[index])
	{
		//printf("DEBUG command_extractor() -> index inicial -> %i\n", index);
		while (is_space(command_input[index])) // ignorar espacios iniciales 
			index++;		
		if (!command_input[index]) // Verificar si llegamos al final después de saltar espacios
			break;

		// CLASIFICACION WORD // OPERATOR
		if (is_operator(command_input[index]))
			word_len = operator_extractor(command, index);  
		else
			word_len = word_extractor(command, index);
			
		// GESTION CASOS ESPECIALES AVANCE INDEX		
		index = advance_index_by_length(index, word_len);

		printf ("DEBUG final proceso command_extractor() \n -> %s, index -> %i, word_len -> %i, LEN COMAND_INPUT -> %i\n",command_input, index, word_len, ft_strlen(command_input));	
		//printf("DEBUG -> command_extractor(). advance index ok\n");				
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

	printf("DEBUG -> word_extractor()\n");

	if (!command->command)
	{
		printf("command_input vacio\n");
		ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO);	 
		return (FAILURE);
	}
		 
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
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
	
	len_input = ft_strlen(command_input);

	printf("Generated word: <%s>\n", command_input);		
	add_word_node(&command->words_list, command_input, WORD);
	printf("DEBUG: add word successfully -> word_len = %d\n\n", len_input);
	
	free(command_input);
	
	return (len_input);
}

/* int word_extractor(t_cmd *command, int start_index)
{
    char *command_input;
    int index;
    int len_input;
    char current_quote;
    bool quote_state;
    char character;
    int debug_counter = 0;  // Para evitar loops infinitos

    if (!command || !command->command)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), GEN_ERROR);

    index = start_index;
    current_quote = 0;
    quote_state = false;
    
    printf("DEBUG word_extractor: start_index=%d, command='%s'\n", 
           start_index, command->command);

    while (command->command[index] && debug_counter < 1000)  // Límite de seguridad
    {
        character = command->command[index];
        
        printf("Index %d: char='%c' quote_state=%s current_quote='%c'\n",
               index, character, quote_state ? "true" : "false", current_quote);
        
        if (is_quote(character))
        {
            if (quote_state == false)
            {
                current_quote = character;
                quote_state = true;
                printf("  -> Entering quotes: %c\n", current_quote);
                index++;
            }
            else if (character == current_quote)
            {
                quote_state = false;
                current_quote = 0;
                printf("  -> Exiting quotes\n");
                index++;
            }
            else
            {
                printf("  -> Different quote inside quotes (literal)\n");
                index++;
            }
        }
        else if (quote_state == false && (is_space(character) || is_operator(character)))
        {
            printf("  -> End of word (space/operator outside quotes)\n");
            break;
        }
        else
        {
            printf("  -> Normal character\n");
            index++;
        }
        
        debug_counter++;
    }
    
    if (debug_counter >= 1000)
    {
        printf("❌ ERROR: Infinite loop detected in word_extractor\n");
        return (GEN_ERROR);
    }
    
    printf("DEBUG: Extracting substring from %d to %d\n", start_index, index);
    
    command_input = ft_substr(command->command, start_index, (index - start_index));
    if (!command_input)
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), GEN_ERROR);
    
    len_input = ft_strlen(command_input);
    printf("DEBUG: Extracted word: '%s' (length=%d)\n", command_input, len_input);
    
    add_word_node(&command->words_list, command_input, WORD);
    free(command_input);
    
    return (len_input);
}
 */
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
