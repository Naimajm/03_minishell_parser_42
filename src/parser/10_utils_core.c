/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_utils_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:24:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/02 21:25:05 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	find_pipe_outside_quotes(char *input, int start_index);
int	find_word_end_outside_quotes(char *input, int start_index);
int find_next_expansion_outside_single_quotes(const char *input, int start_index);
int find_matching_quote_position(const char *input, int quote_start_index);
int	is_quotes_balanced(const char *input);

int	find_pipe_outside_quotes(char *input, int start_index)
{
	int		index;
	bool	in_single_quotes;
	bool	in_double_quotes;

	if (!input || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	index = start_index;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[index])
	{
		if (input[index] == '\'' && in_double_quotes == false)
			in_single_quotes = !in_single_quotes;			// toggle		
		if (input[index] == '\"' && in_single_quotes == false)
			in_double_quotes = !in_double_quotes;			// toggle
		else if (in_single_quotes == false && in_double_quotes == false
			&& is_pipe(input[index]))	// Verificar delimitador solo fuera de todas las comillas
			return (index);
		index++;
	}
	return (index);		// Devuelve final string si no encuentra delimitador
}

int	find_word_end_outside_quotes(char *input, int start_index)
{
	int		index;
	bool	in_single_quotes;
	bool	in_double_quotes;

	if (!input || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	index = start_index;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[index])
	{
		if (input[index] == '\'' && in_double_quotes == false)
			in_single_quotes = !in_single_quotes;			// toggle		
		if (input[index] == '\"' && in_single_quotes == false)
			in_double_quotes = !in_double_quotes;			// toggle
		else if (in_single_quotes == false && in_double_quotes == false
			&& is_word_delimiter(input[index]))	// Verificar delimitador si fuera de todas las comillas
			return (index);
		index++;
	}
	return (index);		// Devuelve final string si no encuentra delimitador
}

int find_next_expansion_outside_single_quotes(const char *input, int start_index)
{
    	int		index;
	bool	in_single_quotes;
	bool	in_double_quotes;

	if (!input || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	index = start_index;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[index])
	{
		if (input[index] == '\'' && in_double_quotes == false)
			in_single_quotes = !in_single_quotes;			// toggle		
		if (input[index] == '\"' && in_single_quotes == false)
			in_double_quotes = !in_double_quotes;			// toggle
		else if (is_expansion_char(input[index]) && in_single_quotes == false) // Verificar delimitador
			return (index);
		index++;
	}
	return (index);				// No encontrada
}

int find_matching_quote_position(const char *input, int quote_start_index)
{
	char	quote;
	int		end_index;

	if (!input || quote_start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	quote = input[quote_start_index];
	end_index = quote_start_index + 1;
	while (input[end_index] && input[end_index] != quote)	// Buscar comilla de cierre
		end_index++;

	if (input[end_index] == quote)
		return (end_index + 1);								// Incluir comilla de cierre
	else
		return (FAILURE);	
}

int	is_quotes_balanced(const char *input)
{
    int	index;
	bool	in_single_quotes;
	bool	in_double_quotes;

	if (!input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	index = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[index])
	{
		if (input[index] == '\'' && in_double_quotes == false)
			in_single_quotes = !in_single_quotes;			// toggle		
		else if (input[index] == '\"' && in_single_quotes == false)
			in_double_quotes = !in_double_quotes;			// toggle		
		index++;
	}
	if (in_single_quotes == false && in_double_quotes == false)
		return (1);
	else
		return (0);
}

/* while (shell->input[index])		// Procesar hasta encontrar pipe fuera de comillas
	{
		character = shell->input[index];		
		if (is_quote(character))							// Manejar comillas
		{
			if (inside_quotes == false) 					// Entrando en comillas
			{				
				active_quote_char = character;
				inside_quotes = true;
			}
			else if (character == active_quote_char) 		// Saliendo de comillas
			{	
				active_quote_char = 0;			
				inside_quotes = false;				
			}
		}		
		else if (inside_quotes == false && is_pipe(character))	// Solo terminar si encontramos pipe fuera de comillas
			break;    
		index++;
	}	 */
