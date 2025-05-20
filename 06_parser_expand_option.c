/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_expand_option.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/20 10:16:58 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// EXPANSION VARIABLES -> GESTION CASO ESPECIAL "$?"
// Devuelve el código de salida del último comando ejecutado

// GESTIONAR CASOS ESPECIALES $$ -> Devuelve el PID (Process ID) del shell actual.
// "$!" -> Devuelve el PID del último proceso ejecutado en segundo plano.
//???? GABI

char	*extract_variable_option(char *input, int index_first_char);
char	*extract_braces_var(char *input, int index_first_char, char first_delimiter, char second_delimiter);

void variables_expander(t_token *token)
{
	int		index;
	char	*substitution_var;
	char	*key;
	char	*value;
	int		first_index;

	if (!token || !token->raw_token)
		return ; 	
	index = 0;	

	// CATEGORIZACION TOKENs -> 7 TIPOS TOKENS
	while (token->raw_token[index])
	{
		substitution_var	= NULL;	
		key			= NULL;
	
		if (token->raw_token[index] == '$')
		{
			first_index = index;

			// GESTION CASO ESPECIAL "$?
			//Devuelve el código de salida del último comando ejecutado
			if (token->raw_token[index + 1] == '?')
			{				
				substitution_var = ft_strdup("$?");
				if (!substitution_var)
					return ;

				// key -> irrelevante
				key = ft_strdup("$?");
				printf("expander / key -> %s\n", key);
				
				add_expand_node(&token->expand_list, substitution_var, first_index,  LAST_EXIT_STATUS);	// añadir nodo expand a token			

				// VALUE = SHELL->EXIT_STATUS
				//value = get_shell_pid(shell);				
			}
			// caso \$VAR -> literal , no inicia expancion
			else if (token->raw_token[index - 1] == '\\')
			{
				substitution_var = extract_variable_option(token->raw_token, first_index);
				if (!substitution_var)
					return ;
				
				// ESTABLECER KEY
				key = ft_strdup(substitution_var);
				printf("expander / key -> %s\n", key);	
				
				// ESTABLCER VALUE COMO LITERAL
				value = ft_strdup(substitution_var);
				printf("expander / value -> %s\n", value);

				add_expand_node(&token->expand_list, substitution_var, first_index, LITERAL);	 // añadir expand nodo a token					
			}
			// caso ${VAR}xx -> literal , no inicia expancion
			else if (token->raw_token[index + 1] == '{')
			{
				substitution_var = extract_braces_var(token->raw_token, first_index, '{', '}');
				if (!substitution_var)
					return ;

				// ESTABLECER KEY
				key = extract_braces_var(substitution_var, 1, '{', '}');
				printf("expander / variable -> %s\n", key);
				
				add_expand_node(&token->expand_list, substitution_var, first_index, CURLY_BRACES);	// añadir token a lista token

				// BUSCAR VALUE			
				//value = get_environment_var(env, key);
				//printf("expander / value -> %s\n", value);				
			}
			else // caso normal -> EXPANSION BASICA -> AÑADIR A LISTA EXPAND
			{				
				substitution_var = extract_variable_option(token->raw_token, first_index);	
				if (!substitution_var)
					return ;

				// ESTABLECER KEY
				key = extract_variable_option(substitution_var, 1);
				printf("expander / key -> %s\n", key);
				
				add_expand_node(&token->expand_list, substitution_var, first_index, BASIC_EXPANSION);	// añadir token a lista token				

				// BUSCAR VALUE			
				//value = get_environment_var(env, key);
				//printf("expander / value -> %s\n", value);
			}	
			index += ft_strlen(substitution_var); // avanza indice hasta final palabra
			free(key);	
			free(substitution_var);				
		}			
		index++;		
	}	
}

char	*extract_variable_option(char *token, int first_index)
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
