/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:24:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/14 13:41:03 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

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