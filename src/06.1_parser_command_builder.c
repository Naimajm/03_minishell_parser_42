/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06.1_parser_command_builder.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:11:43 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/17 00:02:54 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_command	*create_command_node();
static t_command	*find_command_last_node(t_command *commands_list);
void 				print_commands_list(t_command *commands_list);
// static void	print_arguments_list(char **arguments);

void	add_command_node(t_command **commands_list)
{
    t_word_token *new_node;
    t_word_token *last_node;

    if (!commands_list) 							// validation inputs
        return ;   
    
    new_node = create_command_node(); 				// inicializar nuevo nodo token
    if (!new_node)
        return ;		
    last_node = find_word_last_node(*commands_list); // encontrar ultimo nodo y enlazar
    
    if (!last_node)     							
        *commands_list = new_node;   				// caso lista vacio -> añadir en 1º nodo
    else            								    
        last_node->next = new_node;					// lista no vacia  
}

static t_command *create_command_node()
{
    t_command *new_node;

    /* if (!word)
        return (NULL); */

    new_node = (t_command *) malloc(sizeof(t_command));
    if (!new_node)
        print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE);
		
    //new_node->args      = ft_strdup(word);
	new_node->args			= NULL;
    /* if (!new_node->args)
        print_message_and_exit(ERROR_STRUCT_INITIALIZATION, STDERR_FILENO, FAILURE); */

    new_node->infile        = NULL;
    new_node->outfile    	= NULL;

    new_node->redirect_mode	= NULL;
	new_node->delimiter		= NULL;	
    
	new_node->is_btn		= false;
	new_node->exit_status	= 0;

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
        printf("	redirect_mode -> %c // ", command->redirect_mode);
        printf("current -> %p // ", command);
        printf("next -> %p\n", command->next);

        printf("	args      		\n");  
		print_strings_array(command->args);
		
		printf("	infile      	-> %s\n", command->infile);    
		printf("	outfile        	-> %s\n", command->outfile);    

		printf("	redirect_mode   -> %i\n", command->redirect_mode);  
		printf("	delimiter  		-> %s\n", command->delimiter);  

		printf("	is_btn  		-> %i\n", command->delimiter);
		printf("	exit_status  	-> %i\n", command->exit_status);        
        
        print_words_list(command->words_list); 	// IMPRESION LISTA NODOS EXPAND

		//printf("	└──> processed_word         -> %s\n", command); 
		printf("	─────────────────────────────\n\n");
        node_index++;
        command = command->next;
    }
    printf("\n");
}

/* static void	print_arguments_list(char **arguments)
{
	char	*current_arg;
	int 	index;

	if (!arguments)
		return ;
	current_arg = (char *) *arguments;
	while (current_arg)
	{
		ft_printf("args [%i] -> %s\n", index, current_arg);
		index++;
	}	
} */