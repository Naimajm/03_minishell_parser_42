/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/06/10 14:15:01 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	generate_expand_list(t_token *token); 
void	resolve_expansion_values(t_token *token, t_shell *shell);
void	insert_expansion_values(t_token *token);
int		insert_expand_node_value(t_token *token);

void	activate_expand_operators(t_shell *shell)
{
	t_token	*current_token;

	if (!shell->token_list)
		return ;
	current_token = (t_token *) shell->token_list;

	while (current_token)
	{
		if (current_token->type == SINGLE_QUOTES) // no hay expansion $
		{
			current_token = current_token->next;			
			continue;
		}  		
		generate_expand_list(current_token); // GENERAR LISTAS NODOS EXPAND, KEY, VALUE
		
		resolve_expansion_values(current_token, shell); // resolver valores		

		insert_expansion_values(current_token); // INSERTAR VALORE EN TOKEN -> FINAL TOKEN

        //print_expand_list(current_token->expand_list);		
		current_token = current_token->next;
	}	
	//print_token_list(shell->token_list);	
	printf("final activate_expand_operators\n");
}

void generate_expand_list(t_token *token)
{
	int		index;
	int		subs_len;

	if (!token)
		return ; 	
	index 		= 0;		
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

void resolve_expansion_values(t_token *token, t_shell *shell)
{
	t_expand	*expand_node;
	char		*value;

	if (!token || !shell)
		return ;
	expand_node = (t_expand *) token->expand_list;
	while (expand_node)
	{
		//value = NULL;
		if (expand_node->type == LAST_EXIT_STATUS)
			value = ft_itoa(shell->exit_status);		
		else if (expand_node->type == CURLY_BRACES)
			value = get_environment_var(shell->environment, expand_node->key);			
		else if (expand_node->type == LITERAL)
			value = ft_strdup(expand_node->key);
		else
			value = get_environment_var(shell->environment, expand_node->key);
		expand_node->value = value;
		expand_node = expand_node->next;
	}
}

// INSERTAR VALORE EN TOKEN -> FINAL TOKEN
void	insert_expansion_values(t_token *token)
{
    int			 last_position;

    if (!token || !token->expand_list)   // NO HAY EXPANSION VARIABLE
    {		
        token->final_token = ft_strdup(token->raw_token);
        return ;
    }	    
	// insertar valores expandidos de cada nodo
	last_position = insert_expand_node_value(token);

    // Añadir el resto del token después de la última expansión
    if (token->raw_token[last_position])
		token->final_token = ft_strjoin_free(token->final_token, &token->raw_token[last_position]);
    //printf("token->final_token -> %s\n\n", token->final_token);
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
	token->final_token = result;
	//printf("insert_expand_node_value -> %s\n", token->final_token);
	return  (last_position);
}
