/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_parser_struct_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/07 15:03:07 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static t_token	*listtoken_find_last_node(t_token *token_list);
static t_token *token_create_node(char  *input, int type_token);

void	listtoken_add_back(t_token **token_list, char  *input, int token_type)
{
    t_token *new_node;
    t_token *last_node;

    // validation inputs
    if (!token_list || !input)
        return ;   

    // inicializar nuevo nodo token
    new_node = token_create_node(input, token_type);
    if (!new_node)
        return ;
    //ft_printf("addback -> %s\n", new_node->token);

    // encontrar ultimo nodo y enlazar
    last_node = listtoken_find_last_node(*token_list);
    
    if (!last_node)     // caso lista vacio -> añadir en 1º nodo
        *token_list = new_node;   
    else            // lista no vacia      
        last_node->next = new_node;
}

static t_token	*listtoken_find_last_node(t_token *token_list)
{
    // validation 
    if (!token_list)
        return (NULL);

    while (token_list->next)
		token_list = token_list-> next;
    // retorna puntero a ultimo nodo
	return (token_list);
}

static t_token *token_create_node(char  *input, int token_type)
{
    t_token *new_node;

    if (!input)
        return (NULL);

    new_node = (t_token *) malloc(sizeof(t_token));
    if (!new_node)
        return (NULL);
    new_node->token = ft_strdup(input);
    new_node->type  = token_type;
    new_node->next	= NULL;

    //ft_printf("new_node -> %s\n", new_node->token);
    return  (new_node);
}

void print_token_list(t_token *token_list)
{
    if (!token_list)
        return ;    
    while (token_list)
    {
        ft_printf("token -> %s : type -> %d\n", token_list->token, token_list->type);
        token_list = token_list->next;
    }
}
