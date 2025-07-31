/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.2_expand_extractor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:29:59 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 14:10:16 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// FUNCIIONES AUXILIARES EXTRACCION KEY Y BUSQUEDA VALUE
char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
char	*extract_substitution_segment(char *raw_token, int first_index);

// EXTRAER KEY DE SUBSTITUTION_STR
char	*extract_key(char *token, int first_index)
{
	char	*key;
	int		index;

	if (!token)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	index  = first_index;

	// calcular index final variable -> limites > < | " " '"' /0		
	while (!is_space(token[index]) 
		&& !is_operator(token[index]) 
		&& token[index] != '\"' 
		&& token[index] != '\''					
		&& token[index] != '$'
		&& token[index])
		index++;
	
	key = ft_substr(token, first_index, (index - first_index));
	if (!key)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);
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
		if (!match && env[index][ft_strlen(variable)] == '=')			// coincidencia exacta
		{
			value = ft_strdup(&env[index][ft_strlen(variable) + 1]);  	// copiar valor pasado signo '='
			return (value);
		}			
		index++;
	}	
	return (NULL);	
}

char	*extract_substitution_segment(char *raw_token, int first_index)
{
    char	*substitution_str;

	(void)first_index;
    if (!raw_token)
        return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);    
    // Como el tokenizer ya separó correctamente, solo devolvemos todo el token
    substitution_str = ft_strdup(raw_token);
    if (!substitution_str)
        return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);    
    return (substitution_str);
}

