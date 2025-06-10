/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_expand_key_value.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:29:59 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/24 13:41:52 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIIONES AUXILIARES EXTRACCION KEY Y BUSQUEDA VALUE

char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);

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

// INSERTAR VALORE EN TOKEN -> FINAL TOKEN
/* char	*insert_expand_value(char *raw_token, t_expand *expand)
{
	char	*final_token;
	int		index;
	int		index_token;
	int		index_expand;

	if (!raw_token || !expand)
		return (NULL);
	// reserva total
	final_token = (char *) malloc(sizeof(char) * (ft_strlen(raw_token) - (expand->last_index - expand->first_index) + (ft_strlen(expand->value) + 1)));
	index = 0;
	index_token = 0;
	index_expand = 0;
	//ft_printf("expand_var->first_index -> %d\n", expand->first_index);

	// copia token hasta $
	while (index < (expand->first_index - 1))
	{
		final_token[index] = raw_token[index_token];
		index++;
		index_token++;
	}
	// copia valor expandido	
	while (expand->value[index_expand])
	{
		final_token[index] = expand->value[index_expand];
		index++;
		index_expand++;
	}
	index_token = expand->last_index;
	// copia resto token
	while (raw_token[index_token])
	{
		final_token[index] = raw_token[index_token];
		index++;
		index_token++;
	}	
	final_token[index] = '\0'; // terminador nulo
	return (final_token);
} */