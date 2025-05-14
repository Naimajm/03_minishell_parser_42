/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/14 14:12:40 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	activate_expand_operators(t_token *token_list);
void	expand_var_token(t_token *token_list);
t_expand *initialize_expand_stack(void);

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

void	activate_expand_operators(t_token *token_list)
{
	t_token	*ptr_list;

	if (!token_list)
		return ;
	ptr_list = (t_token *) token_list;

	while (ptr_list)
	{
		if (ptr_list->expand_var)		
			expand_var_token(token_list);
		ptr_list = ptr_list->next;
	}	
}

void	expand_var_token(t_token *token_list)
{
	t_expand	*expand_stack;
	int			index;
	int			index_next_char;

	if (!token_list)
		return ;

	expand_stack = initialize_expand_stack();
	index = 0;
	index_next_char = 0;

	// ciclo busqueda $, tomar variable, busqueda y expansion
	while (token_list->token[index] && ft_strchr(token_list->token, '$'))
	{
		if (token_list->token[index] == '$')
		{
			// casos especiales
			if (token_list->token[index + 1] == ' ' || token_list->token[index + 1] == '\"')
				break;

			/// EXTRAER VARIABLE  (similar a word_tokenizer)------------------------------
			// calcular index inicio variable
			expand_stack->first_index = index;

			// calcular index final variable -> limites > < | " " '"' /0		
			while (!is_space(token_list->token[index]) 
				&& !is_operator(token_list->token[index]) 
				&& token_list->token[index] != '\"'
				&& token_list->token[index] != '\''					
				&& token_list->token[index] != '$'
				&& token_list->token[index])
				index++;
			ft_printf("index -> %d\n", index);
			//expand_stack->last_index = expand_stack->first_index ;

			// copiar sub substr
			expand_stack->key = ft_substr(token_list->token, expand_stack->first_index, (index - expand_stack->first_index));
			if (!expand_stack->key)
				return ;
			// validacion
			ft_printf("1 index -> %d, final -> %d\n", expand_stack->first_index, index);
			ft_printf("expand_stack->key -> %s\n", expand_stack->key);

			// GESTION CARACTER $? -> Devuelve el código de salida del último comando ejecutado
			// operadores especiales $? $$ $! -> tratar?
			if (expand_stack->first_index == '?')
				ft_printf("TODO caso especial $?");		

			/// BUSCAR VARIABLE EN ENV ------------------------------


			/// SUSTITUIR VARIABLE POR SU VALOR --------------------
		}

		index++;
	}
	
	
	// verificacion
	print_expand_stack(expand_stack);
	
}



t_expand *initialize_expand_stack(void)
{
	t_expand	*expand_stack;

	expand_stack = (t_expand *) malloc(sizeof(t_expand));
	if(!expand_stack)
		return (NULL);	
		
	expand_stack->first_index	= -1;  	/// posicion de inicio -1 -> error si no se da valor > 0 ??
	expand_stack->last_index 	= -1;	/// posicion final -1 ??
	expand_stack->key	= NULL; // nombre variable extraida
	expand_stack->value	= NULL; // valor de la variable
	return (expand_stack);
}



