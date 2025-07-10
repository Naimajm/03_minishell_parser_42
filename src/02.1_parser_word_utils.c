/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_parser_word_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:44:40 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/10 13:01:04 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token *create_word_node(char  *input, int type_token);
static t_token	*find_word_last_node(t_token *token_list);
void print_tokens_list(t_token *token_list);

void	add_word_node(t_token **token_list, char  *input, int token_type)
{
    t_token *new_node;
    t_token *last_node;

    // validation inputs
    if (!token_list || !input)
        return ;   

    // inicializar nuevo nodo token
    new_node = create_token_node(input, token_type);
    if (!new_node)
        return ;
    //ft_printf("addback -> %s\n", new_node->token);

    // encontrar ultimo nodo y enlazar
    last_node = find_last_token_node(*token_list);
    
    if (!last_node)     // caso lista vacio -> añadir en 1º nodo
        *token_list = new_node;   
    else            // lista no vacia      
        last_node->next = new_node;
}

static t_token *create_word_node(char  *input, int token_type)
{
    t_token *new_node;

    if (!input)
        return (NULL);

    new_node = (t_token *) malloc(sizeof(t_token));
    if (!new_node)
        print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);
    new_node->raw_token      = ft_strdup(input);
    if (!new_node->raw_token)
        print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);

    new_node->type              = token_type;
    new_node->expanded_token    = NULL;
    new_node->noquotes_token    = NULL;

    //new_node->joined_token      = NULL;
    //new_node->require_join      = false;

    new_node->expand_list   = NULL;
    new_node->next	        = NULL;

    //ft_printf("new_node -> %s\n", new_node->token);
    return  (new_node);
}

static t_token	*find_word_last_node(t_token *token_list)
{
    // validation 
    if (!token_list)
        return (NULL);

    while (token_list->next)
		token_list = token_list-> next;
    // retorna puntero a ultimo nodo
	return (token_list);
}

void print_words_list(t_token *token_list)
{
    t_token *token;
    int token_number;

    if (!token_list)
        return ;  
    token = (t_token *)(token_list);
    token_number = 1;
    while (token)
    {
        printf("token [%i]-> %s\n", token_number, token->raw_token);
        printf("type -> %i // ", token->type);
        printf("current -> %p // ", token);
        printf("next -> %p\n", token->next);

        printf("raw_token -> %s\n", token->raw_token);      // VALIDACION SECUENCIA TOKENSVS BASH
        
        print_expand_nodes_list(token->expand_list); // IMPRESION LISTA NODOS EXPAND
        
        printf("expanded_token -> %s\n", token->expanded_token);  // TOKEN YA EXPANDIDO 
        printf("noquotes_token -> %s\n\n", token->noquotes_token);  // TOKEN sin comillas

        token_number++;
        token = token->next;
    }
    printf("\n");
}