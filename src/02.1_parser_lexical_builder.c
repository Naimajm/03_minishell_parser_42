/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_parser_lexical_builder.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:44:40 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 11:59:15 by juagomez         ###   ########.fr       */
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

	if (!word_list || !input) 						// validation inputs
		return ;   
	
	new_node = create_word_node(input, word_type); 	// inicializar nuevo nodo token
	if (!new_node)
		return ;
		
	last_node = find_word_last_node(*word_list); 	// encontrar ultimo nodo y enlazar
	
	if (!last_node)     							// caso lista vacio -> añadir en 1º nodo
		*word_list = new_node;   
	else            								// lista no vacia      
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
	if (!word_list) 					// validation 
		return (NULL);

	while (word_list->next)
		word_list = word_list-> next; 	// retorna puntero a ultimo nodo    
	return (word_list);
}

void print_words_list(t_word_token *word_list)
{
	t_word_token *word;
	int node_index;

	if (!word_list)
		return ;  
	word = (t_word_token *)(word_list);
	node_index = 1;
	while (word)
	{
		printf("\t └───┐\n");
		printf("\t ┌───────────┐\n");
		printf("\t | word [%i]  |\n", node_index);		
		printf("\t └───────────┘\n");
		printf("\t\t word_type -> %c // ", word->word_type);
		printf("current -> %p // ", word);
		printf("next -> %p\n", word->next);

		printf("\t\t raw_word \t\t\t-> %s\n", word->raw_word);    
		
		print_tokens_list(word->tokens_list); // IMPRESION LISTA NODOS EXPAND

		printf("\t\t └──> processed_word \t\t-> %s\n", word->processed_word); 
		printf("\t\t ─────────────────── \n\n");
		node_index++;
		word = word->next;
	}
	printf("\n");
}