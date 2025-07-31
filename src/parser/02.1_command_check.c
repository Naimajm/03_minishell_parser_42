/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_command_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:42:58 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 19:35:27 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int 		validate_pipe_structure(t_cmd *commands_list);
static int 	validate_command_words(t_cmd *command);
int 		validate_redirections_structure(t_cmd *commands_list);
static int 	validate_single_command_redirections(t_cmd *command);
static bool is_redirection_operator(int word_type);

//  VALIDACION SOBRE ESTRUCTURA COMANDOS ----------------------------------

int	validate_command_structure(t_shell *shell)
{		
	t_cmd *commands_list;

	if (!shell)
		return (SYNTAX_ERROR);	
	commands_list = (t_cmd *) shell->commands_list;
        
    if (validate_pipe_structure(commands_list) == SYNTAX_ERROR)
    {
		ft_putendl_fd(ERROR_PIPE_SYNTAX, STDERR_FILENO);
        shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}    
    if (validate_redirections_structure(commands_list) == SYNTAX_ERROR)
    {
		ft_putendl_fd(ERROR_REDIRECTION_SYNTAX, STDERR_FILENO);
        shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}    
    return (SUCCESS); 
}

/// VALIDACION PIPE ---------------------------------------------
int validate_pipe_structure(t_cmd *commands_list)
{
    t_cmd *current_command;
    
    current_command = (t_cmd *) commands_list;
    while (current_command)
    {
        // Comando vacío entre pipes
        if (!current_command->command || ft_strlen(current_command->command) == 0)
            return (SYNTAX_ERROR);
            
        // Verificar que palabras no empiecen/terminen con pipe
        if (validate_command_words(current_command) == SYNTAX_ERROR)
            return (SYNTAX_ERROR);
            
        current_command = current_command->next;
    }    
    return (SUCCESS);
}

static int validate_command_words(t_cmd *command)
{
    t_word *words;
    
    words = command->words_list;
    
    // Error: comando empieza con pipe (no debería pasar con el parser)
    if (words && words->word_type == PIPE)
        return (SYNTAX_ERROR);
        
    // Buscar último word válido
    while (words && words->next)
        words = words->next;
        
    // Error: comando termina con pipe
    if (words && words->word_type == PIPE)
        return (SYNTAX_ERROR);
        
    return (SUCCESS);
}

/// VALIDACION REDIRECTIONS ---------------------------------------------

int validate_redirections_structure(t_cmd *commands_list)
{
    t_cmd *current_command;
    
    current_command = (t_cmd *) commands_list;
    while (current_command)
    {
        if (validate_single_command_redirections(current_command) == SYNTAX_ERROR)
            return (SYNTAX_ERROR);
        current_command = current_command->next;
    }    
    return (SUCCESS);
}

static int validate_single_command_redirections(t_cmd *command)
{
    t_word *current_word;
    
    current_word = (t_word *) command->words_list;
    while (current_word)
    {
        if (is_redirection_operator(current_word->word_type))
        {
            // Error: redirección sin archivo
            if (!current_word->next || current_word->next->word_type != WORD)
                return (SYNTAX_ERROR);
                
            // Error: redirecciones consecutivas
            if (current_word->next && is_redirection_operator(current_word->next->word_type))
                return (SYNTAX_ERROR);
        }
        current_word = current_word->next;
    }    
    return (SUCCESS);
}

static bool is_redirection_operator(int word_type)
{
    return (word_type == OUTFILE || word_type == APPEND || 
            word_type == INFILE || word_type == HERE_DOC);
}