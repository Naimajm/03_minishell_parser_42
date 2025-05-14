/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_parser_expand_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/14 13:51:48 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* int	find_next_char_operator(const char *str, char operator)
{
	int	index;
	int	index_next_char;

	if (!str || !operator)
		return (FAILURE);

	index = 0;
	index_next_char = -1;
	while (str[index])
	{
		if (str[index] == operator)
			index_next_char = str[index + 1];
		index++;
	}
	return (index_next_char);
} */

void print_expand_stack(t_expand *expand_stack)
{
	if (!expand_stack)
		return ;
	ft_printf("expand_stack:\n start -> %d\n", expand_stack->first_index);
	ft_printf("end -> %d\n", expand_stack->last_index);
	ft_printf("key -> %s\n", expand_stack->key);
	ft_printf("value -> %s\n", expand_stack->value);	
}