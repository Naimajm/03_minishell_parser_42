/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_parser_expand_struct.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/06/10 13:58:30 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_expand *create_expand_node(char  *variable, int first_index, int expand_type);
static t_expand	*find_expand_last_node(t_expand *token_list);
void print_expand_list(t_expand *expand_list);

t_expand	*add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type)
{
    t_expand *new_node;
    t_expand *last_node;

    // validation inputs
    if (!expand_list || !substitution_variable)
        return (NULL);   

    // inicializar nuevo nodo token
    new_node = create_expand_node(substitution_variable, first_index, expand_type);
    if (!new_node)
        return (NULL);
    //ft_printf("addback -> %s\n", new_node->token);

    // encontrar ultimo nodo y enlazar
    last_node = find_expand_last_node(*expand_list);
    
    if (!last_node)     // caso lista vacio -> añadir en 1º nodo
        *expand_list = new_node;   
    else
		last_node->next = new_node;
	return (new_node);
}

static t_expand *create_expand_node(char  *substitution_variable, int first_index, int expand_type)
{
    t_expand *new_node;
	
	if (!substitution_variable)
		print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);
	
	new_node = (t_expand *) malloc(sizeof(t_expand));
	if(!new_node)
		print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);	
	new_node->type				= expand_type;		
	new_node->first_index		= first_index;  	
	new_node->substitution_str 	= ft_strdup(substitution_variable); 
	if (!new_node->substitution_str)
		print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);
	new_node->last_index 		= first_index + ft_strlen(substitution_variable) - 1;	
	new_node->key 				= NULL;	 // nombre variable extraida
	new_node->value				= NULL;	

	new_node->next 				= NULL;
    return  (new_node);
}

static t_expand	*find_expand_last_node(t_expand *token_list)
{    
    if (!token_list) // validation 
        return (NULL);

    while (token_list->next)
		token_list = token_list-> next;    
	return (token_list); // retorna puntero a ultimo nodo
}

void print_expand_list(t_expand *expand_list)
{
	t_expand	*expand_node;
	
	if (!expand_list)
		return ;    
	expand_node = (t_expand *)(expand_list);
    while (expand_node)
    {
        printf("expand_stack:\n");
		printf("type -> %d\n", expand_node->type);
		printf("first_index -> %d\n", expand_node->first_index);
		printf("last_index -> %d\n", expand_node->last_index);
		printf("substitution_variable -> %s\n", expand_node->substitution_str);
		printf("key -> %s\n", expand_node->key);
		printf("value -> %s\n", expand_node->value);			
		printf("current node -> %p // next -> %p\n\n", expand_node, expand_node->next);	
        expand_node = expand_node->next;
    }
}
