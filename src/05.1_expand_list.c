/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.1_expand_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/27 18:18:27 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES AUXILIARES EXTRACCION SUBSTITUTION STRING
int		basic_expander(t_token *token, int first_index);
int		last_exit_status_expander(t_token *token, int first_index);
int		curly_braces_expander(t_token *token, int first_index);
int		literal_expander(t_token *token, int first_index);

static int	extract_single_variable_length(char *raw_token, int start_index);

/* int	basic_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;
	char		*key;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	len_input = 0;		
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);	
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, BASIC_EXPANSION);

	key = extract_key(substitution_str, 1); // index 0 -> $	
	//expand_node->key = key;
	expand_node->key = ft_strdup(key);
	if (!expand_node->key)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);

	free(substitution_str);	
	free(key);

	//printf("(basic_expander) len_input -> %i\n", len_input);	
	return (len_input);	
} */

int	basic_expander(t_token *token, int index)
{
    t_expand    *expand_node;
    char		*substitution_str;
    char		*key;
    int			first_index;
    int			subs_len;

    if (!token)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
    
    first_index = index;
    
    // NUEVA LÓGICA: Extraer solo UNA variable, no todo el resto del token
    subs_len = extract_single_variable_length(token->raw_token, index);
    if (subs_len <= 0)
        return (1); // Saltar solo el $ si no es válido
    
    // Extraer la variable individual
    substitution_str = ft_substr(token->raw_token, first_index, subs_len);
    if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
		
	printf("DEBUG: substitution_str = '%s'\n", substitution_str);
    
    expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, BASIC_EXPANSION);
    if (!expand_node)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	printf("DEBUG: expand_node created successfully\n");

	// EXTRACCION KEY -------------------------
    
    // Extraer la key (sin el $) y asignarla
    key = extract_key(substitution_str, 1); // Saltar el $
    if (!key)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	printf("DEBUG: key = '%s'\n", key);

	expand_node->key = ft_strdup(key);
    if (!expand_node->key)
    {
        free(substitution_str);
        free(key);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
    
    // Calcular last_index del nodo expand (como en curly_braces_expander)
    expand_node->last_index = first_index + subs_len - 1;
    
    free(substitution_str);	
    free(key);
    
    return (subs_len);
}

static int	extract_single_variable_length(char *raw_token, int start_index)
{
    int	index;
    
    if (!raw_token || raw_token[start_index] != '$')
        return (0);
        
    index = start_index + 1; // Saltar el $
    
    // Caso $?
    if (raw_token[index] == '?')
    {
        return (2); // $?
    }
    
    // Verificar que empiece con letra o _
    if (!ft_isalpha(raw_token[index]) && raw_token[index] != '_')
    {
        return (1); // Solo el $ (inválido)
    }
    
    // Extraer nombre de variable válido
    while ((ft_isalnum(raw_token[index]) || raw_token[index] == '_') 
        && raw_token[index])
        index++;
    
    return (index - start_index);
}

int	last_exit_status_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char	*substitution_str;
	int		len_input;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	substitution_str = ft_strdup("$?"); // copiar sub substr
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, LAST_EXIT_STATUS);	

	expand_node->key = ft_strdup("$?");
    if (!expand_node->key)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	
	free(substitution_str);
	//ft_printf("token -> %s\n", token);
	return (len_input);
}

int	curly_braces_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			final_index;
	int			len_input;
	char		*key_temp;
	char		*key;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	final_index = first_index + 2; 		// Saltar "${"
	while (token->raw_token[final_index] && token->raw_token[final_index] != '}')
		final_index++;
	if (token->raw_token[final_index] != '}') // No se encontró llave de cierre
		return (FAILURE); 	

	substitution_str = ft_substr(token->raw_token, first_index, (final_index - first_index + 1));
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);

	len_input = ft_strlen(substitution_str);
	//printf("len_input _> %i\n", len_input);
	
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, CURLY_BRACES);
	
	key_temp = extract_key(substitution_str, 1); 	// index 0 -> $
	if (!key_temp)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }

	key = ft_strtrim(key_temp, "{}"); 
	if (!key)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	free(key_temp);
	
	// cortar {}
	expand_node->key = ft_strdup(key_temp);
	if (!expand_node->key)
    {
        free(substitution_str);
        free(key_temp);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	// calcular last_index del nodo expand
	expand_node->last_index = first_index + len_input - 1;
	
	free(substitution_str);	
	free(key);
	return (len_input); //ft_printf("quote_token -> %s\n", token);	
}

int	literal_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;
	char		*key;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
	//ft_printf("substitution_str -> %s\n", substitution_str);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, LITERAL);		

	key = ft_strdup(substitution_str);
	if (!key)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);

	expand_node->key = ft_strdup(key);
	if (!expand_node->key)
    {
        free(substitution_str);
        free(key);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }	
	free(substitution_str);
	free(key);	
	return (len_input);
}
