/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.3_expand_builder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/21 23:03:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_expand *create_expand_node(char  *variable, int first_index, int expand_type);
static t_expand	*find_expand_last_node(t_expand *token_list);
void print_expand_nodes_list(t_expand *expand_list);

t_expand	*add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type)
{
	t_expand *new_node;
	t_expand *last_node;

	// validation inputs
	if (!expand_list || !substitution_variable)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);   

	// inicializar nuevo nodo token
	new_node = create_expand_node(substitution_variable, first_index, expand_type);
	if (!new_node)
		perror_exit(ERROR_EXPAND_INITIALIZATION, STDERR_FILENO, GENERAL_ERROR);
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
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);  
	
	new_node = (t_expand *) malloc(sizeof(t_expand));
	if(!new_node)
		perror_exit(ERROR_EXPAND_INITIALIZATION, STDERR_FILENO, GENERAL_ERROR);	
	new_node->type				= expand_type;		
	new_node->first_index		= first_index;  	
	new_node->substitution_str 	= ft_strdup(substitution_variable); 
	if (!new_node->substitution_str)
		perror_exit(ERROR_EXPAND_INITIALIZATION, STDERR_FILENO, GENERAL_ERROR);
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

void print_expand_nodes_list(t_expand *expand_list)
{
	t_expand	*expand_node;
	int	node_index;
	
	if (!expand_list)
		return ;    
	expand_node = (t_expand *)(expand_list);
	node_index = 1;
	while (expand_node)
	{
		printf("\t\t\t └────┐\n");
		printf("\t\t\t ┌───────────────────┐\n");
		printf("\t\t\t | expand_node: [%i]  |\n", node_index);
		printf("\t\t\t └───────────────────┘\n");
		printf("\t\t\t\t current node -> %p // next -> %p\n", expand_node, expand_node->next);
		printf("\t\t\t\t first_index \t-> %d\n", expand_node->first_index);
		printf("\t\t\t\t last_index \t-> %d\n", expand_node->last_index);
		printf("\t\t\t\t subs_variable \t-> %s\n", expand_node->substitution_str);
		printf("\t\t\t\t key \t\t-> %s\n", expand_node->key);
		printf("\t\t\t\t └──> value \t-> %s\n", expand_node->value);		

		node_index++;
		expand_node = expand_node->next;
	}
}
