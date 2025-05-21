/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_expand_option.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/21 13:29:02 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// EXPANSION VARIABLES -> GESTION CASO ESPECIAL "$?"
// Devuelve el código de salida del último comando ejecutado

// GESTIONAR CASOS ESPECIALES $$ -> Devuelve el PID (Process ID) del shell actual.
// "$!" -> Devuelve el PID del último proceso ejecutado en segundo plano.
//???? GABI

char	*extract_braces_var(char *input, int index_first_char, char first_delimiter, char second_delimiter);



char	*extract_substitution_segment(char *token, int first_index)
{
	char	*variable;
	int		index;

	if (!token)
		return (NULL);
	index = first_index;		
	// longitud de caracteres de la palabra -> limites > < | " " '"' /0
	while (!is_space(token[index])
		&& !is_operator(token[index])
		&& !is_quote(token[index])
		&& token[index])
		index++;
		
	// copiar sub substr
	variable = ft_substr(token, first_index, (index - first_index));
	if (!variable)
		return (NULL);
	//ft_printf("variable -> %s\n", variable);
	return (variable);
}

char	*extract_braces_var(char *token, int index_first_char, char first_delimiter, char second_delimiter)
{
	char	*variable;
	int		index;
	int		second_braces_flag;		// flag validacion sintaxis comillas simples

	if (!token)
		return (NULL);	
	variable	= NULL;
	index 	= find_index_char(token, first_delimiter); // 1º $ 2º {
	second_braces_flag = 0;
	//ft_printf("1º comilla ? -> %c\n", token[index_first_char + 1]);
		
	while (token[index]) // expresion literal -> no interpretacion especial operadores
	{		
		if (token[index] == second_delimiter) // condicion final expresion literal
		{			
			second_braces_flag = 1; // flag 2º comilla -> ok
			break;		
		}
		index++;
	}
	// validacion sintaxis expresion literal
	if (!second_braces_flag)
		return (NULL);

	// copia expresion con comillas simples incluidas
	variable = ft_substr(token, index_first_char, (index - index_first_char + 1)); // + 1-> 2º comillas
	if (!variable)
		return (NULL);
	//ft_printf("quote_token -> %s\n", token);	
	return (variable);
}
