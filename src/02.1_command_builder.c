/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.1_command_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:11:43 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 12:28:58 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_command	*create_command_node(char *input);
static t_command	*find_command_last_node(t_command *commands_list);
void 				print_commands_list(t_command *commands_list);
void	            print_command_arguments(char **args);

void	add_command_node(t_command **commands_list, char *input)
{
	t_command *new_node;
	t_command *last_node;

	if (!commands_list || !input) 						// validation inputs
		return ;   
	
	new_node = create_command_node(input); 				// inicializar nuevo nodo token
	if (!new_node)
		return ;		
	last_node = find_command_last_node(*commands_list); // encontrar ultimo nodo y enlazar
	
	if (!last_node)     								
		*commands_list = new_node;   					// caso lista vacio -> añadir en 1º nodo
	else            								    
		last_node->next = new_node;						// lista no vacia  
}

static t_command *create_command_node(char *input)
{
	t_command *new_node;

	if (!input)
		return (NULL);

	new_node = (t_command *) malloc(sizeof(t_command));
	if (!new_node)
		print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);
		
	new_node->chunk_input = ft_strdup(input);
	if (!new_node->chunk_input)
		print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);

	new_node->args			= NULL;    

	new_node->infile        = NULL;
	new_node->outfile    	= NULL;

	new_node->redirect_mode	    = 0;
	new_node->heredoc_delimiter	= NULL;	
	
	new_node->is_builtin	= false;
	new_node->exit_status	= 0;

	new_node->words_list	= NULL;
	new_node->next	        = NULL;
	//ft_printf("new_word_node -> %s\n", new_node->token);
	return  (new_node);
}

static t_command	*find_command_last_node(t_command *commands_list)
{    
	if (!commands_list) 						// validation 
		return (NULL);
	while (commands_list->next)
		commands_list = commands_list-> next; 	// retorna puntero a ultimo nodo    
	return (commands_list);
}

void print_commands_list(t_command *commands_list)
{
	t_command *command;
	int node_index;

	if (!commands_list)
		return ;  
	command = (t_command *)(commands_list);
	node_index = 1;
	while (command)
	{
		printf("┌──────────────┐\n");
		printf("| command [%i]  |\n", node_index);		
		printf("└──────────────┘\n");
		printf("\t redirect_mode \t-> %c // ", command->redirect_mode);
		printf("current -> %p // ", command);
		printf("next -> %p\n", command->next);

		printf("\t chunck_input \t-> %s\n", command->chunk_input);         
		
		printf("\t infile \t-> %s\n", command->infile);    
		printf("\t outfile \t-> %s\n", command->outfile);    

		printf("\t redirect_mode \t-> %c\n", command->redirect_mode);  
		printf("\t delimiter \t-> %s\n", command->heredoc_delimiter);  

		printf("\t is_btn \t-> %i\n", command->is_builtin);
		printf("\t exit_status \t-> %i\n", command->exit_status);        
		
		print_words_list(command->words_list); 	// IMPRESION LISTA NODOS EXPAND

		print_command_arguments(command->args);
		printf("\t ───────── \n\n");
		node_index++;
		command = command->next;
	}
	printf("\n");
}

void	print_command_arguments(char **args)
{
	int	index;

	if (!args)
		return ;
	index = 0;
	printf("\t └──> args -> [ "); 
	while (args[index])
	{        
		printf("%s ", args[index]); 
		if (args[index + 1])    
			printf(",");   
		index++;
	}
	printf("]\n");
}

