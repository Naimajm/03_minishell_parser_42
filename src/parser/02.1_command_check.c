/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_command_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:42:58 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/01 12:28:49 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//  VALIDACION SOBRE ESTRUCTURA COMANDOS ----------------------------------

int			validate_pipe_structure(t_cmd *command);
int 		validate_redirections_structure(t_cmd *command);

int	validate_command_structure(t_shell *shell)
{		
	t_cmd *current_command;

	if (!shell)
		return (SYNTAX_ERROR);	
	current_command = (t_cmd *) shell->commands_list;
	while (current_command)
	{
		if (validate_pipe_structure(current_command) == SYNTAX_ERROR)
		{
			ft_putendl_fd(ERROR_PIPE_SYNTAX, STDERR_FILENO);
			shell->exit_status = SYNTAX_ERROR;
			return (SYNTAX_ERROR);
		}    
		if (validate_redirections_structure(current_command) == SYNTAX_ERROR)
		{
			ft_putendl_fd(ERROR_REDIRECTION_SYNTAX, STDERR_FILENO);
			shell->exit_status = SYNTAX_ERROR;
			return (SYNTAX_ERROR);
		}
		current_command = current_command->next;
	}       
    return (SUCCESS); 
}

/// VALIDACION PIPE ---------------------------------------------
int validate_pipe_structure(t_cmd *command)
{
	t_word	*first_word;
	t_word	*last_word;

	if (!command->command || ft_strlen(command->command) == 0)	// Comando vacío entre pipes
		return (SYNTAX_ERROR);	

	first_word = (t_word *) command->words_list;    
    if (first_word && first_word->word_type == PIPE)	// Error: comando empieza con pipe
		return (SYNTAX_ERROR);     
    
	last_word = find_word_last_node(command->words_list);		// Buscar último word válido    
	if (last_word && is_pipe(last_word->word_type))		// Error: comando termina con pipe
		return (SYNTAX_ERROR);	      
    return (SUCCESS);
}

/// VALIDACION REDIRECTIONS ---------------------------------------------
int validate_redirections_structure(t_cmd *command)
{
    t_word *current_word;
    
    current_word = (t_word *) command->words_list;
    while (current_word)
    {
		if (current_word->word_type != WORD || current_word->word_type != PIPE)
        //if (is_redirection_operator(current_word->word_type))
        {            
            if (!current_word->next || current_word->next->word_type != WORD)	// Error: redirección sin archivo
                return (SYNTAX_ERROR);            
			if (current_word->next && 
				(current_word->next->word_type != WORD 
				|| current_word->next->word_type != PIPE)) 						// Error: redirecciones consecutivas
                return (SYNTAX_ERROR);
        }
        current_word = current_word->next;
    }    
    return (SUCCESS);
}
