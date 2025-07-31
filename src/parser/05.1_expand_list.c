/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.1_expand_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 11:18:53 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

// FUNCIONES AUXILIARES EXTRACCION SUBSTITUTION STRING
int		basic_expander(t_token *token, int first_index);
int		last_exit_status_expander(t_token *token, int first_index);
int		curly_braces_expander(t_token *token, int first_index);
int		literal_expander(t_token *token, int first_index);

static int	extract_single_variable(char *raw_token, int start_index);

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
    
    // Extraer solo UNA variable, no todo el resto del token
    subs_len = extract_single_variable(token->raw_token, index);
    if (subs_len <= 0)
        return (1); 			// Saltar solo el $ si no es válido
    
    // Extraer la variable individual
    substitution_str = ft_substr(token->raw_token, first_index, subs_len);
    if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    
    expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, BASIC_EXPANSION);
    if (!expand_node)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	// EXTRACCION KEY -------------------------    
    // Extraer la key (sin el $) y asignarla
    key = extract_key(substitution_str, 1); // Saltar el $
    if (!key)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
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

static int	extract_single_variable(char *raw_token, int start_index)
{
    int	index;
    
    if (!raw_token || raw_token[start_index] != '$')
        return (0);	
        
    index = start_index + 1;		// Saltar el $

	// CASO $ UNICO " $  " 
	if (is_space(raw_token[index]) || !raw_token[index])
		return (1);				// Solo el carácter $  
    
    if (raw_token[index] == '?')	// Caso $?
    {
        return (2); 				// $?
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
	substitution_str = ft_strdup("$?");
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
	final_index = first_index + 2; 				// Saltar "${"
	while (token->raw_token[final_index] && token->raw_token[final_index] != '}')
		final_index++;
	if (token->raw_token[final_index] != '}') 	// No se encontró llave de cierre
		return (FAILURE); 	

	substitution_str = ft_substr(token->raw_token, first_index, (final_index - first_index + 1));
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);

	len_input = ft_strlen(substitution_str);
	
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, CURLY_BRACES);
	
	key_temp = extract_key(substitution_str, 1); 	// index 0 -> $
	if (!key_temp)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	key = ft_strtrim(key_temp, "{}"); 				// cortar {}
	if (!key)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	free(key_temp);	
	
	expand_node->key = ft_strdup(key);
	if (!expand_node->key)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	// calcular last_index del nodo expand
	expand_node->last_index = first_index + len_input - 1;
	
	free(substitution_str);	
	free(key);
	return (len_input);
}

int	literal_expander(t_token *token, int index)
{
    t_expand	*expand_node;
    char		*substitution_str;
    int			len_input;
    char		*key;
	int			first_index;

    if (!token)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	
    first_index = index - 1; // coger cchar anterior '\'
		
	len_input = extract_single_variable(token->raw_token, first_index + 1);

    // EXTRAER SOLO LA PARTE DEL ESCAPE
    substitution_str = ft_substr(token->raw_token, first_index, len_input + 1);
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);

    expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, LITERAL);	
	if (!expand_node)
    {
        free(substitution_str);
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
	// FASE EXTRACT KEY !!
	key = ft_substr(substitution_str, 1, ft_strlen(substitution_str) - 1); // Saltar '\'
	if (!key)
    {
        free(substitution_str);        
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }
    // ASIGNAR KEY
    expand_node->key = ft_strdup(key);
    if (!expand_node->key)
    {
		free(key);
        free(substitution_str);        
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
    }      
    expand_node->last_index = index + len_input - 1; 	// Calcular last_index del nodo expand    
	free(key);
    free(substitution_str);	
    return (len_input);
}
