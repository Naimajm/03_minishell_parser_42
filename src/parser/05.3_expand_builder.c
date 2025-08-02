/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.3_expand_builder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:02:16 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/02 15:56:23 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void 		print_expand_nodes_list(t_expand *expand_list);
int			insert_expand_node_value(t_token *token);
t_expand	*add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type);
static t_expand *init_expand_node(char  *variable, int first_index, int expand_type);
static t_expand	*find_expand_last_node(t_expand *token_list);

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

int	insert_expand_node_value(t_token *token)
{
	t_expand	*current_node;
	char		*result;
	char		*prefix;
	int			 last_position;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);  
	current_node = (t_expand *) token->expands_list;
	result = ft_strdup("");
	if (!result)
    	return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
	last_position 	= 0;
	while (current_node)
	{
		if (current_node->first_index > last_position) // Añadir texto antes de la variable
		{
			prefix	= ft_substr(token->raw_token, last_position, current_node->first_index - last_position);
			//printf("  prefix: '%s'\n", prefix);
			result	= ft_strjoin_free(result, prefix);
			free(prefix);
		}       
		result = ft_strjoin_free(result, current_node->value);	 // Añadir el valor expandido
		last_position = current_node->last_index + 1;
		current_node = current_node->next;
	}
	token->expanded_token = result;
	return  (last_position);
}

t_expand	*add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type)
{
	t_expand *new_node;
	t_expand *last_node;

	if (!expand_list || !substitution_variable)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);   

	new_node = init_expand_node(substitution_variable, first_index, expand_type);
	if (!new_node)
		return (ft_putendl_fd(ERROR_EXPAND_INIT, STDERR_FILENO), NULL);
	last_node = find_expand_last_node(*expand_list);	
	if (!last_node)    
		*expand_list = new_node;   
	else
		last_node->next = new_node;
	return (new_node);
}

static t_expand *init_expand_node(char  *substitution_variable, int first_index, int expand_type)
{
	t_expand *new_node;
	
	if (!substitution_variable)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);  
	
	new_node = (t_expand *) malloc(sizeof(t_expand));
	if(!new_node)
		return (ft_putendl_fd(ERROR_EXPAND_INIT, STDERR_FILENO), NULL);	

	new_node->type				= expand_type;		
	new_node->first_index		= first_index;  	
	new_node->substitution_str 	= ft_strdup(substitution_variable); 
	if (!new_node->substitution_str)
		return (ft_putendl_fd(ERROR_EXPAND_INIT, STDERR_FILENO), NULL);
	new_node->last_index 		= first_index + ft_strlen(substitution_variable) - 1;	
	new_node->key 				= NULL;
	new_node->value				= NULL;	
	new_node->next 				= NULL;
	return  (new_node);
}

static t_expand	*find_expand_last_node(t_expand *token_list)
{    
	if (!token_list)
		return (NULL);

	while (token_list->next)
		token_list = token_list-> next;    
	return (token_list);
}
