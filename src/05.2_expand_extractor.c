/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.2_expand_extractor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:29:59 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 12:38:51 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIIONES AUXILIARES EXTRACCION KEY Y BUSQUEDA VALUE

char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
char	*extract_substitution_segment(char *raw_token, int first_index);

// EXTRAER KEY DE SUBSTITUTION_STR
char	*extract_key(char *token, int first_index)
{
	char	*key;
	int	index;

	if (!token)
		return (NULL);
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
		return (NULL);
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
		return (NULL);
	value = NULL;
	index = 0;
	match = 0;
	while (env[index]) // busqueda por key en lista env
	{		
		match = ft_strncmp(env[index], variable, ft_strlen(variable));
		//ft_printf("match ? %d\n", match);
		if (!match)
			value = ft_strdup(&env[index][ft_strlen(variable) + 1]);  // copiar valor pasado signo '='
		index++;
	}
	// CASO NO COINCIDENCIA
	if (!value)
		value = ft_strdup("");     // cambiar NULL a vacio si no hay coincidencia
	//ft_printf("get_env / value -> %s\n", value);
	return (value);	
}

char	*extract_substitution_segment(char *raw_token, int first_index)
{
	char	*substitution_str;
	int		index;

	if (!raw_token)
		return (NULL);
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
		return (NULL);
	//ft_printf("variable -> %s\n", variable);
	return (substitution_str);
}
