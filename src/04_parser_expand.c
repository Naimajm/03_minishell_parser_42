/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/16 17:27:36 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	generate_expand_list(t_token *token); 
void 	resolve_expansion_values(t_token *token, char **environment, int exit_status);
void	insert_expansion_values(t_token *token);
int		insert_expand_node_value(t_token *token);

void	activate_expand_operators(t_word_token *words_list, char **environment, int exit_status)
{
	t_word_token	*current_word;
	t_token			*current_token;

	if (!words_list)
		return ;
	current_word = (t_word_token *) words_list;

	while (current_word)
	{
		current_token = (t_token *) current_word->tokens_list;
		while (current_token)
		{
			//printf("(activate_expand_operators) token->raw_token -> %s\n", current_token->raw_token);	
			generate_expand_list(current_token); 								// GENERAR LISTAS NODOS EXPAND, KEY, VALUE
			resolve_expansion_values(current_token, environment, exit_status); 	// resolver valores	
			insert_expansion_values(current_token); 							// INSERTAR VALORE EN TOKEN -> EXPANDED TOKEN

			//print_expand_list(current_token->expand_list);		
			current_token = current_token->next;
		}	
		current_word = current_word->next;
	}	
	//printf("(activate_expand_operators)\n");
	//print_token_list(shell->token_list);		
}

void generate_expand_list(t_token *token)
{
	int		index;
	int		subs_len;

	if (!token)
		return ; 	
	index 		= 0;	
	
	if (token->type == SINGLE_QUOTES) // Para tokens literales, no hay expansiones        
        return;
	
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
		return ;
	expand_node = (t_expand *) token->expand_list;
	while (expand_node)
	{
		//value = NULL;
		if (expand_node->type == LAST_EXIT_STATUS)
			value = ft_itoa(exit_status);		
		else if (expand_node->type == CURLY_BRACES)
			value = get_environment_var(environment, expand_node->key);			
		else if (expand_node->type == LITERAL)
			value = ft_strdup(expand_node->key);
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
    if (!token || !token->expand_list || token->type == SINGLE_QUOTES)   // tokens literale o NO HAY EXPANSION VARIABLE
    {	
        token->expanded_token = ft_strdup(token->raw_token);
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
		return (0);
	current_node = (t_expand *) token->expand_list;
	result = ft_strdup("");
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
