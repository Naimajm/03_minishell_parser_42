/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.2_expand_extractor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:29:59 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/25 12:03:52 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIIONES AUXILIARES EXTRACCION KEY Y BUSQUEDA VALUE

char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
char	*extract_substitution_segment(char *raw_token, int first_index);
//static int	find_consecutive_variables(char *raw_token, int first_index);

// EXTRAER KEY DE SUBSTITUTION_STR
char	*extract_key(char *token, int first_index)
{
	char	*key;
	int		index;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);	
	//variable = NULL;
	index  = first_index;

	// calcular index final variable -> limites > < | " " '"' /0		
	while (!is_space(token[index]) 
		&& !is_operator(token[index]) 
		&& token[index] != '\"' 
		&& token[index] != '\''					
		&& token[index] != '$'
		&& token[index])
		index++;
	
	// copiar sub substr
	key = ft_substr(token, first_index, (index - first_index));
	if (!key)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);
	//ft_printf("extract_key -> key -> %s\n", key);
	return (key);
}

// BUSCAR VALUE DE KEY
char	*get_environment_var(char **env, char *variable)
{
	char	*value;
	int		index;
	int 	match;

	if (!env || !variable)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);	
	value = NULL;
	index = 0;
	match = 0;
	while (env[index]) // busqueda por key en lista env
	{		
		match = ft_strncmp(env[index], variable, ft_strlen(variable));
		//ft_printf("match ? %d\n", match);
		if (!match && env[index][ft_strlen(variable)] == '=')			// coincidencia exacta
		{
			value = ft_strdup(&env[index][ft_strlen(variable) + 1]);  	// copiar valor pasado signo '='
			return (value);
		}			
		index++;
	}
	// CASO NO COINCIDENCIA
	//ft_printf("get_environment_var / value -> %s\n", value);
	return (NULL);	
}

//	 FUNCIONM ORIGINAL !! NO ARREGLA VARIABLES CONSECUTIVAS	$USER$TERM
char	*extract_substitution_segment(char *raw_token, int first_index)
{
	char	*substitution_str;
	int		index;

	if (!raw_token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	index = first_index;		
	// longitud de caracteres de la palabra -> limites > < | " " '"' /0
	while (!is_space(raw_token[index])
		&& !is_operator(raw_token[index])
		&& !is_quote(raw_token[index])
		&& raw_token[index])
		index++;
		
	// copiar sub substr
	substitution_str = ft_substr(raw_token, first_index, (index - first_index));
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), NULL);
	//ft_printf("variable -> %s\n", variable);
	return (substitution_str);
}

/* char	*extract_substitution_segment(char *raw_token, int first_index)
{
	char	*substitution_str;
	int		final_index;
	//int		raw_len;

	if (!raw_token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	final_index = first_index;	
	
	// CASO 2 VARIABLES $ A EXPANDIR -> $USER$TERM
	if (raw_token[final_index] == '$' && raw_token[final_index + 1])
		final_index = find_consecutive_variables(raw_token, final_index);
	// CASO SIN $ VARIABLE EXPANDIDA
	else
	{		
		while (!is_space(raw_token[final_index])
			&& !is_operator(raw_token[final_index])
			&& !is_quote(raw_token[final_index])
			//&& raw_token[index] != '$' 		// para variables a expandir consecutivas
			&& raw_token[final_index])
			final_index++;
	}	

	// GESTION CASOS ESPECIALES AVANCE INDEX
	//final_index = advance_index_by_length(final_index, raw_len);

	// 🔧 FIX: Validar índices antes de ft_substr
    if (final_index <= first_index)
    {
        printf("❌ ERROR: índices inválidos first_index=%d, final_index=%d\n", 
               first_index, final_index);
        return (ft_strdup(""));  // Retornar string vacío en lugar de NULL
    }
		
	printf("len total -> %i // final_index-> %i", ft_strlen(raw_token) , final_index);
	substitution_str = NULL;

	// copiar sub substr
	//substitution_str = ft_substr(raw_token, first_index, (final_index - first_index));
	if (!substitution_str)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);
	//ft_printf("variable -> %s\n", variable);
	return (substitution_str);
} */

/* static int	find_consecutive_variables(char *raw_token, int first_index)
{
	int index;

	index = first_index;
	index++;						// saltar el caracter $
	if (raw_token[index] == '?') 	// Caso especial: $?
		index++;
	else
	{
		// Añadir && raw_token[index] != '$' para parar en siguiente $
		while (!is_space(raw_token[index])
			&& !is_operator(raw_token[index])
			&& !is_quote(raw_token[index])
			&& raw_token[index] != '$'
			&& raw_token[index])
			index++;
	}
	return (index);
} */

/* char *extract_substitution_segment(char *raw_token, int first_index)
{
    char    *substitution_str;
    int     index;
    int     len_input;

    if (!raw_token)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), GENERAL_ERROR);
    
    index = first_index;
    
    // Algoritmo inspirado en word_tokenizer()
    if (raw_token[index] == '$' && raw_token[index + 1])
    {
        index++; // Saltar $
        
        if (raw_token[index] == '?')
            index++; // Caso $?
        else
        {
            // Extraer variable como noquotes_tokenizer
            while (!is_space(raw_token[index]) 
                && !is_operator(raw_token[index]) 
                && !is_quote(raw_token[index])
                && raw_token[index] != '$'
                && raw_token[index])
                index++;
        }
    }
    else
    {
        // Texto normal como noquotes_tokenizer
        while (!is_space(raw_token[index])
            && !is_operator(raw_token[index]) 
            && !is_quote(raw_token[index]) 
            && raw_token[index])
            index++;
    }
    
    len_input = index - first_index;
    if (len_input <= 0)
        return (ft_strdup(""));
    
    substitution_str = ft_substr(raw_token, first_index, len_input);
    if (!substitution_str)
        return (ft_putendl_fd(ERROR_MEMORY_ALLOCATION, STDERR_FILENO), GENERAL_ERROR);
    
    return (substitution_str);
} */

/* static int	find_consecutive_variables(char *raw_token, int first_index)
{
    int index;
    int token_len;

    if (!raw_token)
        return (first_index);
    
    token_len = ft_strlen(raw_token);
    
    // 🔧 FIX: Validar que first_index esté dentro de límites
    if (first_index >= token_len || first_index < 0)
        return (first_index);
    
    index = first_index;
    
    // 🔧 FIX: Verificar que realmente hay un $
    if (raw_token[index] != '$')
        return (first_index + 1);
    
    index++;  // saltar el caracter $
    
    // 🔧 FIX: Verificar límites después de saltar $
    if (index >= token_len)
        return (index);
    
    if (raw_token[index] == '?') // Caso especial: $?
    {
        index++;
    }
    else
    {
        // 🔧 FIX: Añadir verificación de límites en el while
        while (index < token_len &&
               !is_space(raw_token[index]) &&
               !is_operator(raw_token[index]) &&
               !is_quote(raw_token[index]) &&
               raw_token[index] != '$' &&
               raw_token[index])
            index++;
    }
    
    return (index);
} */

	
    
