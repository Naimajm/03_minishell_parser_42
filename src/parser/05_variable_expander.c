/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_variable_expander.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 11:18:48 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

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
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
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
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));  
	
	extract_expansion_nodes(token);			// GENERAR LISTAS NODOS EXPAND, KEY, VALUE
	resolve_expansion_values(token, environment, exit_status);	// resolver valores	
	insert_expansion_values(token);			// INSERTAR VALORE EN TOKEN -> EXPANDED TOKEN
}

void	extract_expansion_nodes(t_token *token)
{
    int		index;
    int		subs_len;

    if (!token)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));    
    if (token->type == SINGLE_QUOTES) // Para tokens literales, no hay expansiones
		return ;
		
	index = 0;   
    while (token->raw_token[index]) // CATEGORIZACION EXPAND NODOS
    {
        if (token->raw_token[index] == '$')
        {
            // Caso $ sin nada después O $ seguido de espacio -> NO ES EXPANSIÓN, saltar
            if (!token->raw_token[index + 1] || is_space(token->raw_token[index + 1]) || token->raw_token[index + 1] == '"')
            {
                index++; // tratar $ como texto normal
            }
            // Caso $ seguido de carácter válido para expansión
            else if (token->raw_token[index + 1])
            {
                if (token->raw_token[index + 1] == '?') // GESTION CASO ESPECIAL "$?
                    subs_len = last_exit_status_expander(token, index);			
                else if (index > 0 && token->raw_token[index - 1] == '\\') // caso \$VAR -> literal , no inicia expancion
                    subs_len = literal_expander(token, index);
                else if (token->raw_token[index + 1] == '{') // caso ${VAR}xx
                    subs_len = curly_braces_expander(token, index);
                else 							// caso normal -> EXPANSION BASICA 
                    subs_len = basic_expander(token, index);				
                
                if (subs_len == FAILURE)		// error
                    return ;			
                index += subs_len;	
            }
        }	
        else	
            index++;							 // resto caracteres
    }
}

void resolve_expansion_values(t_token *token, char **environment, int exit_status)
{
	t_expand	*expand_node;
	char		*value;

	if (!token || !environment)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));  
	expand_node = (t_expand *) token->expands_list;
	while (expand_node)
	{
		if (expand_node->type == LAST_EXIT_STATUS)
		{
			value = ft_itoa(exit_status);	
			if (!value)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
		}				
		else if (expand_node->type == CURLY_BRACES)
			value = get_environment_var(environment, expand_node->key);			
		else if (expand_node->type == LITERAL)
		{
			value = ft_strdup(expand_node->key);
			if (!value)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
		}			
		else
			value = get_environment_var(environment, expand_node->key);
		expand_node->value = value;
		expand_node = expand_node->next;
	}
}

// INSERTAR VALOR EN TOKEN -> FINAL TOKEN
void	insert_expansion_values(t_token *token)
{
	int			 last_position;

	if (!token || !token->expands_list || token->type == SINGLE_QUOTES)   // tokens literales o NO HAY EXPANSION VARIABLE
	{	
		token->expanded_token = ft_strdup(token->raw_token);
		if (!token->expanded_token)
			return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));
		return ;
	}			
	last_position = insert_expand_node_value(token); // insertar valores expandidos de cada nodo

	// Añadir el resto del token después de la última expansión
	if (token->raw_token[last_position])
		token->expanded_token = ft_strjoin_free(token->expanded_token, &token->raw_token[last_position]);
}

int	insert_expand_node_value(t_token *token)
{
	t_expand	*current_node;
	char		*result;
	char		*prefix;
	int			 last_position;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);  
	current_node = (t_expand *) token->expands_list;
	result = ft_strdup("");
	if (!result)
    	return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
	last_position 	= 0;
	while (current_node)
	{
		if (current_node->first_index > last_position) // Añadir texto antes de la variable
		{
			prefix	= ft_substr(token->raw_token, last_position, current_node->first_index - last_position);
			printf("  prefix: '%s'\n", prefix);
			result	= ft_strjoin_free(result, prefix);
			free(prefix);
		}       
		result = ft_strjoin_free(result, current_node->value);	 // Añadir el valor expandido
		last_position = current_node->last_index + 1;
		current_node = current_node->next;
	}
	token->expanded_token = result;
	return  (last_position);
}
