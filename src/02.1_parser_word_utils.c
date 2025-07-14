/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_parser_word_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:44:40 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/14 17:29:49 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_word_token *create_word_node(char  *input, char word_type);
static t_word_token	*find_word_last_node(t_word_token *word_list);
void print_words_list(t_word_token *word_list);

void	add_word_node(t_word_token **word_list, char  *input, char word_type)
{
    t_word_token *new_node;
    t_word_token *last_node;

    // validation inputs
    if (!word_list || !input)
        return ;   

    // inicializar nuevo nodo token
    new_node = create_word_node(input, word_type);
    if (!new_node)
        return ;
    //ft_printf("addback -> %s\n", new_node->token);

    // encontrar ultimo nodo y enlazar
    last_node = find_word_last_node(*word_list);
    
    if (!last_node)     // caso lista vacio -> añadir en 1º nodo
        *word_list = new_node;   
    else            // lista no vacia      
        last_node->next = new_node;
}

static t_word_token *create_word_node(char  *input, char word_type)
{
    t_word_token *new_node;

    if (!input)
        return (NULL);

    new_node = (t_word_token *) malloc(sizeof(t_word_token));
    if (!new_node)
        print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);
		
    new_node->raw_word      = ft_strdup(input);
    if (!new_node->raw_word)
        print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);

    new_node->word_type         = word_type;
    new_node->processed_word    = NULL;

    new_node->tokens_list	= NULL;
    new_node->next	        = NULL;

    //ft_printf("new_word_node -> %s\n", new_node->token);
    return  (new_node);
}

static t_word_token	*find_word_last_node(t_word_token *word_list)
{
    // validation 
    if (!word_list)
        return (NULL);

    while (word_list->next)
		word_list = word_list-> next;
    // retorna puntero a ultimo nodo
	return (word_list);
}

void print_words_list(t_word_token *word_list)
{
    t_word_token *word_token;
    int node_index;

    if (!word_list)
        return ;  
    word_token = (t_word_token *)(word_list);
    node_index = 1;
    while (word_token)
    {
        printf("word [%i]\n", node_index);
        printf("word_type -> %c // ", word_token->word_type);
        printf("current -> %p // ", word_token);
        printf("next -> %p\n", word_token->next);

        printf("raw_word -> %s\n\n", word_token->raw_word);    
        
        print_tokens_list(word_token->tokens_list); // IMPRESION LISTA NODOS EXPAND

		printf("processed_word -> %s\n", word_token->processed_word); 
        node_index++;
        word_token = word_token->next;
    }
    printf("\n");
}