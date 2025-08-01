/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_utils_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:24:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/01 17:20:55 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_operator_length(char *input, int index)
{
	int	len;

	if (!input || !input[index])
        return (0);		
	len = 0;
    if (input[index] == '>' && input[index + 1] == '>')
		return (len = 2); 				// >>
    if (input[index] == '<' && input[index + 1] == '<')
		return (len = 2); 				// <<
    if (input[index] == '>' || input[index] == '<')
		return (len = 1); 				// > o < 
	if (input[index] == '|')
        return (len = 1);
    return (len);
}

int is_pipe(char character)
{
	int	pipe;

	if (!character)
		return (0);
	pipe = 0;
	if (character == '|')
		pipe = 1;
	return (pipe);
}

int	is_quote(char character)
{
	int	quote;

	quote = 0;
	if (character == '\'' || character == '\"')
		quote = 1;
	return (quote);
}

int is_redirection(char character)
{
	int	operator;

	operator = 0;
	if (character == '>' || character == '<')
		operator = 1;
	return (operator);
}
