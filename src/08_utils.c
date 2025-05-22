/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:24:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/22 12:48:04 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int find_index_char(const char *str, char character)
{
    int index;    

    index = 0;
    while (str[index])
    {
        if (character == str[index])
            return (index);
        index++; 
    }
    return (0);
}

int	is_quote(char character)
{
	int	quote;

	quote = 0;
	if (character == '\'' || character == '\"')
		quote = 1;
	return (quote);
}

int is_operator(char character)
{
	int	operator;

	operator = 0;
	if (character == '>' || character == '<' || character == '|')
		operator = 1;
	return (operator);
}

int	is_space(char character)
{
	int	space;

	space = 0;
	if (character == ' ' || (character >= '\t' && character <= '\r'))
		space = 1;
	return (space);
}