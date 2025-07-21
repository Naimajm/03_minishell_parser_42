/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_variable_expander.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/21 13:20:14 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_single_token(t_token *token, char **environment, int exit_status);
void	extract_expansion_nodes(t_token *token); 
void 	resolve_expansion_values(t_token *token, char **environment, int exit_status);
void	insert_expansion_values(t_token *token);
int		insert_expand_node_value(t_token *token);

void	variable_expander(t_word *words_list, char **environment, int exit_status)
{
	t_word	*current_word;
	t_token	*current_token;

	if (!words_list)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);
	current_word = (t_word *) words_list;

	while (current_word)
	{
		current_token = (t_token *) current_word->tokens_list;
		while (current_token)
		{
			expand_single_token(current_token, environment, exit_status);
			current_token = current_token->next;
		}	
		current_word = current_word->next;
	}		
}

void	expand_single_token(t_token *token, char **environment, int exit_status)
{
	if (!token)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);  
	
	extract_expansion_nodes(token);			// GENERAR LISTAS NODOS EXPAND, KEY, VALUE
	resolve_expansion_values(token, environment, exit_status);	// resolver valores	
	insert_expansion_values(token);			// INSERTAR VALORE EN TOKEN -> EXPANDED TOKEN
}

void	extract_expansion_nodes(t_token *token)
{
	int		index;
	int		subs_len;

	if (!token)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);  	
	index 		= 0;	
	
	if (token->type == SINGLE_QUOTES) // Para tokens literales, no hay expansiones        
		return ;
	
	while (token->raw_token[index]) // CATEGORIZACION EXPAND NODOS
	{
		if (token->raw_token[index] == '$' && token->raw_token[index + 1])
		{
			if (token->raw_token[index + 1] == '?') // GESTION CASO ESPECIAL "$?
				subs_len = last_exit_status_expander(token, index);			
			else if (index > 0 && token->raw_token[index - 1] == '\\') // caso \$VAR -> literal , no inicia expancion
				subs_len = literal_expander(token, index);
			else if (token->raw_token[index + 1] == '{') // caso ${VAR}xx
				subs_len = curly_braces_expander(token, index);
			else // caso normal -> EXPANSION BASICA -> AÑADIR A LISTA EXPAND
				subs_len = basic_expander(token, index);				
			if (subs_len == FAILURE)	// error
				return ;			
			index += subs_len;	
		}	
		else	
			index++;	 // $sin caracter adicional o resto caracteres
	}
}

void resolve_expansion_values(t_token *token, char **environment, int exit_status)
{
	t_expand	*expand_node;
	char		*value;

	if (!token || !environment)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);  
	expand_node = (t_expand *) token->expands_list;
	while (expand_node)
	{
		if (expand_node->type == LAST_EXIT_STATUS)
		{
			value = ft_itoa(exit_status);	
			if (!value)
				perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
		}				
		else if (expand_node->type == CURLY_BRACES)
			value = get_environment_var(environment, expand_node->key);			
		else if (expand_node->type == LITERAL)
		{
			value = ft_strdup(expand_node->key);
			if (!value)
				perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
		}			
		else
			value = get_environment_var(environment, expand_node->key);
		expand_node->value = value;
		expand_node = expand_node->next;
	}
}

// INSERTAR VALORE EN TOKEN -> FINAL TOKEN
void	insert_expansion_values(t_token *token)
{
	int			 last_position;

	//printf("(insert_expansion_values) token->raw_token -> %s\n", token->raw_token);	
	if (!token || !token->expands_list || token->type == SINGLE_QUOTES)   // tokens literales o NO HAY EXPANSION VARIABLE
	{	
		token->expanded_token = ft_strdup(token->raw_token);
		if (!token->expanded_token)
			perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
		//printf("(insert_expansion_values) token->expanded_token -> %s\n\n", token->expanded_token);
		return ;
	}			
	last_position = insert_expand_node_value(token); // insertar valores expandidos de cada nodo

	// Añadir el resto del token después de la última expansión
	if (token->raw_token[last_position])
		token->expanded_token = ft_strjoin_free(token->expanded_token, &token->raw_token[last_position]);
	//printf("(insert_expansion_values) token->expanded_token -> %s\n\n", token->expanded_token);
}

int	insert_expand_node_value(t_token *token)
{
	t_expand	*current_node;
	char		*result;
	char		*prefix;
	int			 last_position;

	if (!token)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, FAILURE);  
	current_node = (t_expand *) token->expands_list;
	result = ft_strdup("");
	if (!result)
    	perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
	last_position 	= 0;
	while (current_node)
	{
		if (current_node->first_index > last_position) // Añadir texto antes de la variable
		{
			prefix	= ft_substr(token->raw_token, last_position, current_node->first_index - last_position);
			result	= ft_strjoin_free(result, prefix);
			free(prefix);
		}       
		result = ft_strjoin_free(result, current_node->value);	 // Añadir el valor expandido
		last_position = current_node->last_index + 1;
		current_node = current_node->next;
	}
	token->expanded_token = result;
	//printf("insert_expand_node_value -> %s\n", token->final_token);
	return  (last_position);
}
