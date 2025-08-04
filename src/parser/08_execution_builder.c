/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_execution_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/05 00:14:48 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static void	extract_command_arguments(t_cmd *command);
static void	fill_command_arguments(t_cmd *command);
static void	process_redirections(t_cmd *command);
static int	count_word_arguments(t_word *words_list);
static bool	is_builtin_command(char *command);

void    build_execution_structure(t_cmd *commands_list, t_shell *shell)
{
	t_cmd * current_command;
	t_word	*current_word;

	if (!commands_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_command = (t_cmd *) commands_list;
	while (current_command)
	{
		// AÑADIR WORD A LISTA DE ARGUMENTOS		
		fill_command_arguments(current_command);

		// PROCESAR REDIRECCIONAMIENTOS
		current_word = (t_word *) current_command->words_list;
		while (current_word)
		{
			if (current_word->word_type != WORD 
					&& current_word->word_type != PIPE 
					&& current_word->next->word_type == WORD)
			{
				process_redirections(current_command);	
				current_word = current_word->next;
			}			
			current_word = current_word->next;
		}
		

		//extract_command_arguments(current_command);
		//process_redirections(current_command);	
		
		// gestion EXIT STATUS ¡¡
		if (current_command->exit_status == ERROR)
			shell->exit_status = ERROR;
		else
			shell->exit_status = SUCCESS;
		current_command = current_command->next;
	}    
}

static void	fill_command_arguments(t_cmd *command)
{
	int		index;
	int 	args_count;
	t_word * current_word;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	index       	= 0;
	current_word 	= (t_word *) command->words_list;

	// CREACION LISTA ARGS
	args_count = count_word_arguments(current_word); 	// calculo total argumentos -> WORD (no operadores)
	if (!command->args)
		command->args = (char **) malloc(sizeof(char *) * (args_count + 2));	// reserva memoria total argumentos
	else
		while (command->args[index])
			index++;
	
	// CICLO PROCESAMIENTO LISTA WORDS
	while (current_word)
    {
        if (current_word->word_type == WORD)
        {
            command->args[index] = ft_strdup(current_word->processed_word);
            if (!command->args[index])
                return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
            index++;
        }
        else if (current_word->word_type != WORD && current_word->word_type != PIPE)
        {
            // 🔍 SALTAR OPERADOR DE REDIRECCIÓN Y SU ARCHIVO
            current_word = current_word->next; // Saltar operador (< > >> <<)
            if (current_word && current_word->word_type == WORD)
                current_word = current_word->next; // Saltar nombre de archivo
            continue;
        }
        current_word = current_word->next;
    }
	command->args[index] = NULL; 					// terminador nulo char **   	

	if (command->args[0])							// verificar si es comando builtin
        command->is_btn = is_builtin_command(command->args[0]);
}

/* static void    extract_command_arguments(t_cmd *command)
{
	int index;
	int args_count;
	t_word *current_word;

	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	index       = 0;
	current_word = (t_word *) command->words_list;
	
	args_count = count_word_arguments(current_word); 	// calculo total argumentos -> WORD (no operadores)

	// reserva memoria total argumentos
	command->args = (char **) malloc(sizeof(char *) * (args_count + 1));
	if (!command->args)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
	
	current_word = (t_word *) command->words_list; 	// Llenar argv con processed_word
	while (current_word)
	{
		if (current_word->word_type == WORD)
		{
			command->args[index] = ft_strdup(current_word->processed_word);
			if (!command->args[index])
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
			index++;
		}
		current_word = current_word->next;
	}
	command->args[index] = NULL; 					// terminador nulo char **   	
	if (command->args[0])							// verificar si es comando builtin
        command->is_btn = is_builtin_command(command->args[0]);
} */

static void    process_redirections(t_cmd *command)
{
    t_word *current_word;

    if (!command)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));        
    current_word = command->words_list;
    while (current_word)
    {
		//printf("debug extract_redirections() commando-.> %s / tipo %c\n", current_word->processed_word, current_word->word_type);
        if (current_word->word_type == OUTFILE)
        {            
            if (current_word->next && current_word->next->word_type == WORD)
				process_outfile(command, current_word);
        }
        else if (current_word->word_type == APPEND) 
        {            
            if (current_word->next && current_word->next->word_type == WORD)
				process_append(command, current_word);
        }
        else if (current_word->word_type == INFILE)
        {
            if (current_word->next && current_word->next->word_type == WORD)
				process_infile(command, current_word);
        }
        else if (current_word->word_type == HERE_DOC)
        {
            if (current_word->next && current_word->next->word_type == WORD)
           		process_heredoc(command, current_word);
        }  
		/* if (command->exit_status == 1)
			shell->exit_status = 1;
		else
			shell->exit_status = 0;   */    
        current_word = current_word->next;
    }    
}

// calculo total argumentos -> WORD (no operadores)
static int count_word_arguments(t_word *words_list)
{
	t_word    *current_word;
	int args_count;

	if(!words_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), ERROR);
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

// Verificar si commando es builtin
static bool is_builtin_command(char *command)
{
    char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    int 	index;
	bool	is_builtin;
	
	if (!command)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), false);
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
