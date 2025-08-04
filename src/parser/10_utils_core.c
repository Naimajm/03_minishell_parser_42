/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_utils_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:24:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/04 13:11:39 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	find_pipe_outside_quotes(char *input, int start_index);
int find_redirection_outside_quotes(char *input, int start_index);
int	find_word_end_outside_quotes(char *input, int start_index);
int find_next_expansion_outside_single_quotes(const char *input, int start_index);
int find_matching_quote_position(const char *input, int quote_start_index);

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
		update_quote_state(input[index], &in_single_quotes, &in_double_quotes);
		if (in_single_quotes == false && in_double_quotes == false
			&& is_pipe(input[index]))	// Verificar delimitador solo fuera de todas las comillas
			return (index);
		index++;
	}
	return (index);		// Devuelve final string si no encuentra delimitador
}

int find_redirection_outside_quotes(char *input, int start_index)
{
    int index;
    bool in_single_quotes;
    bool in_double_quotes;

    if (!input || start_index < 0)
        return (ft_strlen(input));  // Retorna final si no encuentra        
    index = start_index;
    in_single_quotes = false;
    in_double_quotes = false;    
    while (input[index])
    {
		update_quote_state(input[index], &in_single_quotes, &in_double_quotes);
    	if (!in_single_quotes && !in_double_quotes && is_redirection(input[index]))
            return (index);
        index++;
    }    
    return (index);  // Final de string
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
		update_quote_state(input[index], &in_single_quotes, &in_double_quotes);
		if (in_single_quotes == false && in_double_quotes == false
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

	if (!input || start_index < 0 || start_index >= (int) ft_strlen(input))
		return (FAILURE);	
	index = start_index;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[index])
	{
		update_quote_state(input[index], &in_single_quotes, &in_double_quotes);
		if (is_expansion_char(input[index]) && in_single_quotes == false) // Verificar delimitador
			return (index);
		index++;
	}
	return (FAILURE);					
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

