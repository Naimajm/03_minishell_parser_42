/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_syntax.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 12:16:41 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    extract_command_arguments(t_command *command);
void    extract_redirections(t_command *command);
int     count_word_arguments(t_word_token *words_list);

void    build_execution_structure(t_command *commands_list)
{
	t_command * current_command;

	if (!commands_list)
		return ;
	current_command = (t_command *) commands_list;
	while (current_command)
	{
		extract_command_arguments(current_command);
		extract_redirections(current_command);

		current_command = current_command->next;
	}    
}

void    extract_command_arguments(t_command *command)
{
	int index;
	int args_count;
	t_word_token *current_word;

	if (!command)
		return ;
	index       = 0;
	args_count  = 0;
	current_word = (t_word_token *) command->words_list;
	
	count_word_arguments(current_word); 	// calculo total argumentos -> WORD (no operadores)

	// reserva memoria total argumentos
	command->args = (char **) malloc(sizeof(char *) * (args_count + 1));
	if (!command->args)
		return ;
	
	current_word = (t_word_token *) command->words_list; 	// Llenar argv con processed_word
	while (current_word)
	{
		if (current_word->word_type == WORD)
		{
			command->args[index] = ft_strdup(current_word->processed_word);
			if (!command->args[index])
				return ;
			index++;
		}
		current_word = current_word->next;
	}
	command->args[index] = NULL; // terminador nulo char **    
}

void    extract_redirections(t_command *command)
{
	t_word_token    *current_word;

	current_word = (t_word_token *) command->words_list;
	while (current_word)
	{
		
		if (current_word->word_type != WORD)
			command->redirect_mode = current_word->word_type;	// etiquetar modo redirect

		if (current_word->word_type == OUTFILE || current_word->word_type == APPEND) 	// CASO REDIRECCION OUT
		{            
			if (current_word->next && current_word->next->word_type == WORD)
				command->outfile = ft_strdup(current_word->next->processed_word);     
		}         
		else if (current_word->word_type == INFILE) 								// CASO REDIRECCION IN -> INFILE
		{
			if (current_word->next && current_word->next->word_type == WORD)
				command->infile = ft_strdup(current_word->next->processed_word);                  
		}
		else if (current_word->word_type == HERE_DOC) 								// CASO REDIRECCION delimitador
		{
			if (current_word->next && current_word->next->word_type == WORD)
				command->heredoc_delimiter = ft_strdup(current_word->next->processed_word);         
		}        
		current_word = current_word->next;
	}    
}

// FUNCIONES AUXILIARES

// calculo total argumentos -> WORD (no operadores)
int count_word_arguments(t_word_token *words_list)
{
	t_word_token    *current_word;
	int args_count;

	current_word    = (t_word_token *) words_list;
	args_count      = 0;
	while (current_word)
	{
		if (current_word->word_type == WORD)
			args_count++;
		current_word = current_word->next;
	}   
	return (args_count);
}


