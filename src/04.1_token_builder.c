/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04.1_token_builder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/25 12:39:01 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token	*find_token_last_node(t_token *token_list);
static t_token *create_token_node(char  *input, int type_token);
void print_tokens_list(t_token *token_list);

void	add_token_node(t_token **token_list, char  *word, int token_type)
{
	t_token *new_node;
	t_token *last_node;
	
	if (!token_list || !word)                       
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));      
	new_node = create_token_node(word, token_type); // inicializar nuevo nodo token
	if (!new_node)
		return (ft_putendl_fd(ERROR_TOKEN_INIT, STDERR_FILENO));
	//ft_printf("token -> %s\n", new_node->token);    
	last_node = find_token_last_node(*token_list);  // encontrar ultimo nodo y enlazar
	
	if (!last_node)                                 // caso lista vacio -> añadir en 1º nodo
		*token_list = new_node;   
	else                                            // lista no vacia      
		last_node->next = new_node;
}

static t_token *create_token_node(char  *word, int token_type)
{
	t_token *new_node;

	if (!word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);  

	new_node = (t_token *) malloc(sizeof(t_token));
	if (!new_node)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);
	new_node->raw_token      = ft_strdup(word);
	if (!new_node->raw_token)
		return (ft_putendl_fd(ERROR_TOKEN_INIT, STDERR_FILENO), NULL);

	new_node->type              = token_type;
	new_node->expanded_token    = NULL;
	new_node->noquotes_token    = NULL;

	new_node->expands_list   = NULL;
	new_node->next	        = NULL;

	//ft_printf("new_node -> %s\n", new_node->token);
	return  (new_node);
}

static t_token	*find_token_last_node(t_token *token_list)
{	
	if (!token_list)
		return (NULL);   // validation 

	while (token_list->next)
		token_list = token_list-> next; // retorna puntero a ultimo nodo    
	return (token_list);
}

void print_tokens_list(t_token *token_list)
{
	t_token *token;
	int node_index;

	if (!token_list)
		return ;  
	token = (t_token *)(token_list);
	node_index = 1;
	while (token) 
	{
		printf("\t\t └───┐\n");
		printf("\t\t ┌───────────┐\n");
		printf("\t\t | token [%i] |\n", node_index);
		printf("\t\t └───────────┘\n");
		printf("\t\t\t type -> %i // ", token->type);
		printf("current -> %p // next -> %p\n", token, token->next);

		printf("\t\t\t raw_token \t\t-> %s\n", token->raw_token);         
		
		print_expand_nodes_list(token->expands_list);                            // IMPRESION LISTA NODOS EXPAND 
		
		printf("\t\t\t expanded_token \t-> %s\n", token->expanded_token);   // TOKEN YA EXPANDIDO         
		printf("\t\t\t └──> noquotes_token \t-> %s\n", token->noquotes_token); // TOKEN sin comillas

		node_index++;
		token = token->next;
	}
	printf("\n");
}
