/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_parser_expand_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/09 15:08:51 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void print_expand_stack(t_expand *expand_stack)
{
	if (!expand_stack)
		return ;
	ft_printf("expand_stack:\n start -> %d\n", expand_stack->start);
	ft_printf("end -> %d\n", expand_stack->end);
	ft_printf("key -> %s\n", expand_stack->key);
	ft_printf("value -> %s\n", expand_stack->value);	
}