/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_execution_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/22 11:30:27 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    extract_command_arguments(t_cmd *command);
void    extract_redirections(t_cmd *command);
int     count_word_arguments(t_word *words_list);
bool 	is_builtin_command(char *command);

void    build_execution_structure(t_cmd *commands_list)
{
	t_cmd * current_command;

	if (!commands_list)
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	current_command = (t_cmd *) commands_list;
	while (current_command)
	{
		extract_command_arguments(current_command);
		extract_redirections(current_command);

		current_command = current_command->next;
	}    
}

void    extract_command_arguments(t_cmd *command)
{
	int index;
	int args_count;
	t_word *current_word;

	if (!command)
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	index       = 0;
	current_word = (t_word *) command->words_list;
	
	args_count = count_word_arguments(current_word); 	// calculo total argumentos -> WORD (no operadores)

	// reserva memoria total argumentos
	command->args = (char **) malloc(sizeof(char *) * (args_count + 1));
	if (!command->args)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	
	current_word = (t_word *) command->words_list; 	// Llenar argv con processed_word
	while (current_word)
	{
		if (current_word->word_type == WORD)
		{
			command->args[index] = ft_strdup(current_word->processed_word);
			if (!command->args[index])
				print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
			index++;
		}
		current_word = current_word->next;
	}
	command->args[index] = NULL; // terminador nulo char **    

	// verificar si es comando builtin
	if (command->args[0])
        command->is_btn = is_builtin_command(command->args[0]);
}

void    extract_redirections(t_cmd *command)
{
    t_word *current_word;

    if (!command)
        print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);	
        
    current_word = command->words_list;
    while (current_word)
    {
        if (current_word->word_type == OUTFILE) 	// CASO REDIRECCION OUT
        {            
            if (current_word->next && current_word->next->word_type == WORD)
                command->outfile = ft_strdup(current_word->next->processed_word);
        }
        else if (current_word->word_type == APPEND) 	// CASO REDIRECCION APPEND
        {            
            if (current_word->next && current_word->next->word_type == WORD)
            {
                command->outfile = ft_strdup(current_word->next->processed_word);
                command->append = true;
            }
        }
        else if (current_word->word_type == INFILE) 	// CASO REDIRECCION IN -> INFILE
        {
            if (current_word->next && current_word->next->word_type == WORD)
                command->infile = ft_strdup(current_word->next->processed_word);
        }
        else if (current_word->word_type == HERE_DOC) 	// CASO REDIRECCION delimitador
        {
            if (current_word->next && current_word->next->word_type == WORD)
            {
                command->delimiter = ft_strdup(current_word->next->processed_word);
                command->hd = true;
            }
        }        
        current_word = current_word->next;
    }    
}

// FUNCIONES AUXILIARES

// Verificar si commando es builtin
bool is_builtin_command(char *command)
{
    char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    int 	index;
	bool	is_builtin;
	
	if (!command)
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	index 		= 0;
	is_builtin 	= false;        
    while (builtins[index])
    {
		if (ft_strncmp(command, builtins[index], ft_strlen(command)) == 0)
            is_builtin = true;
        index++;
    }
    return (is_builtin);
}

// calculo total argumentos -> WORD (no operadores)
int count_word_arguments(t_word *words_list)
{
	t_word    *current_word;
	int args_count;

	if(!words_list)
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);

	current_word    = (t_word *) words_list;
	args_count      = 0;
	while (current_word)
	{
		if (current_word->word_type == WORD)
			args_count++;
		current_word = current_word->next;
	}   
	return (args_count);
}


