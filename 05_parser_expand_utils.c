/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_parser_expand_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/21 14:01:55 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static t_expand *expand_create_node(char  *variable, int first_index, int expand_type);
static t_expand	*expand_find_last_node(t_expand *token_list);
void print_expand_list(t_expand *expand_list);

void	add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type)
{
    t_expand *new_node;
    t_expand *last_node;

    // validation inputs
    if (!expand_list || !substitution_variable)
        return ;   

    // inicializar nuevo nodo token
    new_node = expand_create_node(substitution_variable, first_index, expand_type);
    if (!new_node)
        return ;
    //ft_printf("addback -> %s\n", new_node->token);

    // encontrar ultimo nodo y enlazar
    last_node = expand_find_last_node(*expand_list);
    
    if (!last_node)     // caso lista vacio -> añadir en 1º nodo
        *expand_list = new_node;   
    else            // lista no vacia      
        last_node->next = new_node;
}

static t_expand *expand_create_node(char  *substitution_variable, int first_index, int expand_type)
{
    t_expand *new_node;
	
	if (!substitution_variable)
		return (NULL);
	
	new_node = (t_expand *) malloc(sizeof(t_expand));
	if(!new_node)
		return (NULL);	
	new_node->type				= expand_type;		
	new_node->first_index		= first_index;  	
	new_node->substitution_str 	= ft_strdup(substitution_variable); 
	if (!new_node->substitution_str)
		return (NULL);
	new_node->last_index 		= first_index + ft_strlen(substitution_variable);	
	new_node->key 				= NULL;	 // nombre variable extraida
	new_node->value				= NULL;	

	new_node->next 				= NULL;
    return  (new_node);
}

static t_expand	*expand_find_last_node(t_expand *token_list)
{
    // validation 
    if (!token_list)
        return (NULL);

    while (token_list->next)
		token_list = token_list-> next;
    // retorna puntero a ultimo nodo
	return (token_list);
}

void print_expand_list(t_expand *expand_list)
{
	t_expand	*expand;
	
	if (!expand_list)
		return ;    
	expand = (t_expand *)(expand_list);
    while (expand)
    {
        printf("expand_stack:\n");
		printf("type -> %d\n", expand->type);
		printf("first_index -> %d\n", expand->first_index);
		printf("last_index -> %d\n", expand->last_index);
		printf("substitution_variable -> %s\n", expand->substitution_str);
		printf("key -> %s\n", expand->key);
		printf("value -> %s\n", expand->value);			
		printf("current node -> %p // next -> %p\n\n", expand, expand->next);	
        expand = expand->next;
    }
}

void print_expand_stack(t_expand *expand_stack)
{
	t_expand	*ptr_expand;

	if (!expand_stack)
		return ;
	ptr_expand = (t_expand *)(expand_stack);
	ft_printf("expand_stack:\nfirst_index -> %d\n", ptr_expand->first_index);
	ft_printf("last_index -> %d\n", ptr_expand->last_index);
	ft_printf("key -> %s\n", ptr_expand->key);
	ft_printf("value -> %s\n", ptr_expand->value);	
}