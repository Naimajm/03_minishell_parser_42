/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.2_command_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:11:43 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/30 22:04:49 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_cmd	*create_command_node(char *input);
static t_cmd	*find_command_last_node(t_cmd *commands_list);
void 			print_commands_list(t_cmd *commands_list);
void	        print_command_arguments(char **args);
void			print_output(char **args);

void	add_command_node(t_cmd **commands_list, char *input)
{
	t_cmd *new_node;
	t_cmd *last_node;

	if (!commands_list || !input) 		
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));  
	
	new_node = create_command_node(input); 					
	if (!new_node)
		return (ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO));
		
	last_node = find_command_last_node(*commands_list); // encontrar ultimo nodo y enlazar
	if (!last_node)     								
		*commands_list = new_node;   					// caso lista vacio -> añadir en 1º nodo
	else            								    
		last_node->next = new_node;						// lista no vacia
}

static t_cmd *create_command_node(char *input)
{
	t_cmd *new_node;

	if (!input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	
	new_node = (t_cmd *) malloc(sizeof(t_cmd));
	if (!new_node)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);		
	new_node->command = ft_strdup(input);
	if (!new_node->command)
		return (ft_putendl_fd(ERROR_COMMAND_INIT, STDERR_FILENO), NULL);

	new_node->args			= NULL;    

	new_node->infile        = NULL;
	new_node->outfile    	= NULL;
	new_node->delimiter		= NULL;

	new_node->append		= false;
	new_node->hd			= false;	
	new_node->is_btn		= false;
	new_node->exit_status	= SUCCESS;

	new_node->words_list	= NULL;
	new_node->next	        = NULL;
	return  (new_node);
}

static t_cmd	*find_command_last_node(t_cmd *commands_list)
{    
	if (!commands_list) 					
		return (NULL);
	while (commands_list->next)
		commands_list = commands_list-> next; 	// retorna puntero a ultimo nodo   
	return (commands_list);
}

void print_commands_list(t_cmd *commands_list)
{
	t_cmd *command;
	int node_index;

	if (!commands_list)
		return ;
	command = (t_cmd *)(commands_list);
	node_index = 1;
	while (command)
	{
		printf("┌──────────────┐\n");
		printf("| command [%i]  |\n", node_index);		
		printf("└──────────────┘\n");
		printf("\t command \t-> %s\n", command->command);   
		printf("\t\t current -> %p // ", command);
		printf("next -> %p\n", command->next);		      
		
		printf("\t infile \t-> %s\n", command->infile);    
		printf("\t outfile \t-> %s\n", command->outfile);  
		printf("\t delimiter \t-> %s\n", command->delimiter);   

		printf("\t append \t-> %d\n", command->append);  
		printf("\t hd \t\t-> %d\n", command->hd);  		

		printf("\t is_btn \t-> %d\n", command->is_btn);
		printf("\t exit_status \t-> %d\n", command->exit_status);        
		
		print_words_list(command->words_list); 	// IMPRESION LISTA NODOS EXPAND

		print_command_arguments(command->args);
		printf("\t ───────── \n\n");

		print_output(command->args);			// IMPRESION OUTPUT
		node_index++;
		command = command->next;
	}
	printf("\n");
}

void	print_command_arguments(char **args)
{
	int	index;

	if (!args)
    {
        printf("\t └──> args -> [ (null) ]\n");  // No es error, solo información
        return;
    }
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

void	print_output(char **args)
{
	int	index;

	if (!args)
    {
        printf("└──> OUTPUT -> (null)\n");  // No es error, solo información
        return;
    }
	index = 0;
	printf("└──> OUTPUT -> ("); 
	while (args[index])
	{        
		printf("%s", args[index]);
		if (args[index + 1])    
			printf(" ");   
		index++;
	}
	printf(")\n"); 
}


