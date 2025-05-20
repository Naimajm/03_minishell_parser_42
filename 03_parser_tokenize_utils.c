/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_parser_tokenize_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:26:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/20 09:49:41 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static t_token	*token_find_last_node(t_token *token_list);
static t_token *token_create_node(char  *input, int type_token);
void print_token_list(t_token *token_list);

void	add_token_node(t_token **token_list, char  *input, int token_type)
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
    last_node = token_find_last_node(*token_list);
    
    if (!last_node)     // caso lista vacio -> añadir en 1º nodo
        *token_list = new_node;   
    else            // lista no vacia      
        last_node->next = new_node;
}

static t_token *token_create_node(char  *input, int token_type)
{
    t_token *new_node;

    if (!input)
        return (NULL);

    new_node = (t_token *) malloc(sizeof(t_token));
    if (!new_node)
        return (NULL);
    new_node->raw_token      = ft_strdup(input);
    if (!new_node->raw_token)
        return (NULL);
    new_node->type          = token_type;
    new_node->final_token   = NULL;

    new_node->expand_list   = NULL;
    new_node->next	        = NULL;

    //ft_printf("new_node -> %s\n", new_node->token);
    return  (new_node);
}

static t_token	*token_find_last_node(t_token *token_list)
{
    // validation 
    if (!token_list)
        return (NULL);

    while (token_list->next)
		token_list = token_list-> next;
    // retorna puntero a ultimo nodo
	return (token_list);
}

void print_token_list(t_token *token_list)
{
    t_token *ptr_token;

    if (!token_list)
        return ;  
    ptr_token = (t_token *)(token_list);
    while (ptr_token)
    {
        ft_printf("token -> %s [type %d]\n", ptr_token->raw_token, ptr_token->type);
        ft_printf("current -> %p // ", ptr_token);
        ft_printf("next -> %p\n\n", ptr_token->next);

        // IMPRESION LISTA NODOS EXPAND
        print_expand_list(ptr_token->expand_list);

        ptr_token = ptr_token->next;
    }
}
