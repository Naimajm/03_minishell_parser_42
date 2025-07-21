/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_syntax_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:15:07 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/22 00:21:24 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//  VALIDACION POST ESTRUCTURAS ----------------------------------

static int validate_command_semantics(t_cmd *commands_list)
{
    t_cmd *current;
    
    current = commands_list;
    while (current)
    {
        // Validar que hay al menos una palabra WORD
        if (!has_executable_word(current))
            return (SYNTAX_ERROR);
            
        // Validar redirecciones múltiples del mismo tipo
        if (validate_multiple_redirections(current) == SYNTAX_ERROR)
            return (SYNTAX_ERROR);
            
        current = current->next;
    }
    
    return (SUCCESS);
}

static bool has_executable_word(t_cmd *command)
{
    t_word *current_word;
    
    current_word = command->words_list;
    while (current_word)
    {
        if (current_word->word_type == WORD)
            return (true);
        current_word = current_word->next;
    }
    
    return (false);
}

static int validate_multiple_redirections(t_cmd *command)
{
    int outfile_count = 0;
    int infile_count = 0;
    t_word *current_word;
    
    current_word = command->words_list;
    while (current_word)
    {
        if (current_word->word_type == OUTFILE || current_word->word_type == APPEND)
            outfile_count++;
        else if (current_word->word_type == INFILE || current_word->word_type == HERE_DOC)
            infile_count++;
            
        current_word = current_word->next;
    }
    
    // En bash, múltiples redirecciones del mismo tipo usan la última
    // Pero podemos validar si queremos ser más estrictos
    return (SUCCESS);
}


//  VALIDACION SOBRE COMANDOS ----------------------------------

char *validate_command_structure(t_cmd *commands_list)
{
    static char error_buffer[256];
    
    if (!commands_list)
    {
        ft_strlcpy(error_buffer, "minishell: syntax error: empty command", sizeof(error_buffer));
        return (error_buffer);
    }
    
    if (validate_pipe_structure(commands_list) == SYNTAX_ERROR)
    {
        ft_strlcpy(error_buffer, "minishell: syntax error: invalid pipe sequence", sizeof(error_buffer));
        return (error_buffer);
    }
    
    if (validate_redirections_structure(commands_list) == SYNTAX_ERROR)
    {
        ft_strlcpy(error_buffer, "minishell: syntax error: invalid redirection", sizeof(error_buffer));
        return (error_buffer);
    }
    
    return (NULL); // Sin errores
}

/// VALIDACION PIPE ---------------------------------------------

static int validate_pipe_structure(t_cmd *commands_list)
{
    t_cmd *current;
    
    current = commands_list;
    while (current)
    {
        // Comando vacío entre pipes
        if (!current->command || ft_strlen(current->command) == 0)
            return (SYNTAX_ERROR);
            
        // Verificar que palabras no empiecen/terminen con pipe
        if (validate_command_words(current) == SYNTAX_ERROR)
            return (SYNTAX_ERROR);
            
        current = current->next;
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

static int validate_redirections_structure(t_cmd *commands_list)
{
    t_cmd *current;
    
    current = commands_list;
    while (current)
    {
        if (validate_single_command_redirections(current) == SYNTAX_ERROR)
            return (SYNTAX_ERROR);
        current = current->next;
    }
    
    return (SUCCESS);
}

static int validate_single_command_redirections(t_cmd *command)
{
    t_word *current_word;
    
    current_word = command->words_list;
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