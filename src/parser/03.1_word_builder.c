/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03.1_word_builder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:44:40 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/02 12:18:23 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void 			print_words_list(t_word *word_list);
static void		add_word_node(t_word **word_list, char  *input, char word_type);
static t_word 	*init_word_node(char  *input, char word_type);
t_word			*find_word_last_node(t_word *word_list);

void print_words_list(t_word *word_list)
{
	t_word *word;
	int node_index;

	if (!word_list)
		return ;  
	word = (t_word *)(word_list);
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

int		create_word(t_word **word_list, char *command_input, int start_index, int word_len, int word_type)
{
	char	*word_input;

	if (!word_list || start_index < 0 || word_len <= 0)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);	
	word_input = ft_substr(command_input, start_index, word_len);
	if (!word_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);		
	add_word_node(word_list, word_input, word_type);
	free(word_input);
	return(word_len);
}

static void	add_word_node(t_word **word_list, char  *input, char word_type)
{
	t_word *new_node;
	t_word *last_node;

	if (!word_list || !input) 						
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));  
	
	new_node = init_word_node(input, word_type); 	
	if (!new_node)
		return (ft_putendl_fd(ERROR_WORD_INIT, STDERR_FILENO));
		
	last_node = find_word_last_node(*word_list); 	// encontrar ultimo nodo y enlazar	
	if (!last_node)     							// caso lista vacio -> añadir en 1º nodo
		*word_list = new_node;   
	else            								// lista no vacia      
		last_node->next = new_node;
}

static t_word *init_word_node(char  *input, char word_type)
{
	t_word *new_node;

	if (!input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);

	new_node = (t_word *) malloc(sizeof(t_word));
	if (!new_node)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);
		
	new_node->raw_word      	= ft_strdup(input);
	if (!new_node->raw_word)
		return (ft_putendl_fd(ERROR_WORD_INIT, STDERR_FILENO), NULL);

	new_node->word_type         = word_type;
	new_node->processed_word    = NULL;

	new_node->tokens_list		= NULL;
	new_node->next	        	= NULL;
	return  (new_node);
}

t_word	*find_word_last_node(t_word *word_list)
{    
	if (!word_list) 					
		return (NULL);

	while (word_list->next)
		word_list = word_list-> next;
	return (word_list);
}

