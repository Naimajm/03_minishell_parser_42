/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_lexical_analyzer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:37:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/02 21:33:06 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	command_extractor(t_cmd *command);
static int	word_extractor(t_cmd *process_list, int start_index);
static int	operator_extractor(t_cmd *process_list, int start_index);

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

static int	command_extractor(t_cmd *command)
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

static int	word_extractor(t_cmd *command, int start_index)
{
	int	word_end_position;
	int	word_len;

	if (!command->command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE); 	
	
	// DELIMITADOR LIMITES CARACTERES WORD -------------------------------------------
	word_end_position = find_word_end_outside_quotes(command->command, start_index);
	if (word_end_position == FAILURE)
		return (ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO), FAILURE); 
		
	word_len = word_end_position - start_index;
	if (word_len > 0)
		// CREAR NODO TOKEN ------------------------------------------------------
		create_word(&command->words_list, command->command, start_index, word_len, WORD);	
	return (word_len);
}

static int	operator_extractor(t_cmd *command, int start_index)
{
	char	*input;
	int		operator_len;	

	if (!command || start_index < 0)
		return (FAILURE);
	input = command->command;
	operator_len = get_operator_length(command->command, start_index);

	if (operator_len == 2 && input[start_index] == '>')
		create_word(&command->words_list, ">>", start_index, operator_len, APPEND);	
    else if (operator_len == 2 && input[start_index] == '<')
		create_word(&command->words_list, "<<", start_index, operator_len, HERE_DOC);
    else if (operator_len == 1 && input[start_index] == '>')
		create_word(&command->words_list, ">", start_index, operator_len, OUTFILE);	
    else if (operator_len == 1 && input[start_index] == '<')
		create_word(&command->words_list, "<", start_index, operator_len, INFILE);	
    else if (operator_len == 1 && is_pipe(input[start_index]))
		create_word(&command->words_list, "|", start_index, operator_len, PIPE);
	else
		return (FAILURE); 
	return (operator_len);
}
