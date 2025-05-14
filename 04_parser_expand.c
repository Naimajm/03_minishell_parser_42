/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/14 09:03:59 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	expand_var_token(t_token *token_list);
t_expand *initialize_expand_stack(void);
int		search_expand_operators(t_token *token_list);

void	activate_expand_operators(t_token *token_list)
{
	t_token	*temp_list;

	if (!token_list)
		return ;
	temp_list = (t_token *) token_list;

	while (temp_list)
	{
		if (temp_list->expand_var)
		{
			// GESTION CARACTER $? -> Devuelve el código de salida del último comando ejecutado
			// operadores especiales $? $$ $! -> tratar?
			if (ft_strnstr(temp_list->token, "$?", ft_strlen(temp_list->token)))
				ft_printf("TODO gestion $?\n");	
			else
				expand_var_token(token_list);
		}
		temp_list = temp_list->next;
	}	
}

void	expand_var_token(t_token *token_list)
{
	t_expand	*expand_stack;
	//int			index;

	if (!token_list)
		return ;

	expand_stack = initialize_expand_stack();
	//index = 0;
	
	// verificacion
	print_expand_stack(expand_stack);
	
}

t_expand *initialize_expand_stack(void)
{
	t_expand	*expand_stack;

	expand_stack = (t_expand *) malloc(sizeof(t_expand));
	if(!expand_stack)
		return (NULL);	
		
	expand_stack->start	= -1;  	/// posicion de inicio -1 -> error si no se da valor > 0 ??
	expand_stack->end 	= -1;	/// posicion final -1 ??
	expand_stack->key	= NULL; // nombre variable extraida
	expand_stack->value	= NULL; // valor de la variable
	return (expand_stack);
}

int	search_expand_operators(t_token *token_list)
{
	t_token	*temp_list;
	int	match;

	if (!token_list)
		return (0);
	temp_list = (t_token *) token_list;
	match = 0;
	while (temp_list)
	{
		if (ft_strchr(temp_list->token, '$'))
		{
			match = 1;
			temp_list->expand_var = true;   // activar funcion expansion en ese token			
		}			
		temp_list = temp_list->next;
	}
	return (match);	
}

