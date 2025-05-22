/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/22 12:47:53 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void generate_expand_list(t_token *token);
//static int	determine_expansion_type(const char *token, int index);

char	*insert_expand_value(char *token, t_expand *expand_var);
char	*extract_variable(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
t_expand *initialize_expand_stack(void);

void	activate_expand_operators(t_shell *shell)
{
	t_token	*current_token;

	if (!shell->token_list)
		return ;
	current_token = (t_token *) shell->token_list;

	while (current_token)
	{
		if (current_token->type == SINGLE_QUOTES)  // no hay expansion $
			continue;

		// GENERAR LISTAS NODOS EXPAND
		generate_expand_list(current_token);
			

		// establecer KEY
		
		// BUSCAR VALUE -
		// shell->env
		// shell->exit-status



		// PREFIJO +  INSERTAR VALUE - QUITAR SUBSTITUTION_VAR + SUFIJO


		current_token = current_token->next;
	}	
}

void generate_expand_list(t_token *token)
{
	int		index;
	char	*substitution_str;
	char	*key;
	//char	*value;
	int		first_index;

	if (!token || !token->raw_token)
		return ; 	
	index = 0;	

	// CATEGORIZACION TOKENs -> 7 TIPOS TOKENS
	while (token->raw_token[index])
	{
		substitution_str	= NULL;	
		key			= NULL;
	
		if (token->raw_token[index] == '$')
		{
			first_index = index;

			// GESTION CASO ESPECIAL "$?
			//Devuelve el código de salida del último comando ejecutado
			if (token->raw_token[index + 1] == '?')
			{				
				substitution_str = ft_strdup("$?");
				if (!substitution_str)
					return ;

				// key -> irrelevante
				key = ft_strdup("$?");
				//printf("expander / key -> %s\n", key);
				
				add_expand_node(&token->expand_list, substitution_str, first_index,  LAST_EXIT_STATUS);	// añadir nodo expand a token			

				// VALUE = SHELL->EXIT_STATUS
				//value = get_shell_pid(shell);				
			}
			// caso \$VAR -> literal , no inicia expancion
			else if (token->raw_token[index - 1] == '\\')
			{
				substitution_str = extract_substitution_segment(token->raw_token, first_index);
				if (!substitution_str)
					return ;
				
				// ESTABLECER KEY
				key = ft_strdup(substitution_str);
				//printf("expander / key -> %s\n", key);	
				
				// ESTABLCER VALUE COMO LITERAL
				//value = ft_strdup(substitution_str);
				//printf("expander / value -> %s\n", value);

				add_expand_node(&token->expand_list, substitution_str, first_index, LITERAL);	 // añadir expand nodo a token					
			}
			// caso ${VAR}xx -> literal , no inicia expancion
			else if (token->raw_token[index + 1] == '{')
			{
				substitution_str = extract_braces_var(token->raw_token, first_index, '{', '}');
				if (!substitution_str)
					return ;

				// ESTABLECER KEY
				key = extract_braces_var(substitution_str, 1, '{', '}');
				//printf("expander / variable -> %s\n", key);
				
				add_expand_node(&token->expand_list, substitution_str, first_index, CURLY_BRACES);	// añadir token a lista token

				// BUSCAR VALUE			
				//value = get_environment_var(env, key);
				//printf("expander / value -> %s\n", value);				
			}
			else // caso normal -> EXPANSION BASICA -> AÑADIR A LISTA EXPAND
			{				
				substitution_str = extract_substitution_segment(token->raw_token, first_index);	
				if (!substitution_str)
					return ;

				// ESTABLECER KEY
				key = extract_substitution_segment(substitution_str, 1);
				//printf("expander / key -> %s\n", key);
				
				add_expand_node(&token->expand_list, substitution_str, first_index, BASIC_EXPANSION);	// añadir token a lista token				

				// BUSCAR VALUE			
				//value = get_environment_var(env, key);
				//printf("expander / value -> %s\n", value);
			}	
			index += ft_strlen(substitution_str); // avanza indice hasta final palabra
			free(key);	
			free(substitution_str);				
		}			
		index++;		
	}	
}

/* static int	determine_expansion_type(const char *token, int index)
{
	int	expansion_type;

	if (!token || !index)
		return (0);
	if (token[index + 1] == '?')
		expansion_type = LAST_EXIT_STATUS;
	else if (token[index + 1] == '{')
		expansion_type = CURLY_BRACES;		
	else if (token[index - 1] == '\\')
		expansion_type = LITERAL;
	else
		expansion_type = BASIC_EXPANSION;
	return (expansion_type);
} */

/* void	expand_token(t_token *token, char **env)
{
	t_expand	*expand;
	int			index;

	if (!token || !token->raw_token)
		return ;
	expand = initialize_expand_stack();
	if (!expand)
		return ;

	// copia raw a final token
	token->final_token = ft_strdup(token->raw_token);
	if (!token->final_token)
	{
		free_expand_stack(expand);
		return ;
	}
	index = 0;
	// ciclo busqueda $, tomar variable, busqueda y expansion
	while (token->final_token[index] && ft_strchr(token->final_token, '$'))
	{
		//index = 0;		
		if (token->final_token[index] == '$')
		{
			/// SWITCH OPERADOR $ + VARIABLE  (casos especiales)-------------------------
			// casos especiales '$ ' y '$"' y  final linea '$\0'
			if (token->final_token[index + 1] == ' '
				|| token->final_token[index + 1] == '\"'
				|| token->final_token[index + 1] == '\0')
			{
				index++;	// literal $ -> avanzar
				continue;
			}		
			// GESTION CARACTER $? -> Devuelve el código de salida del último comando ejecutado
			// operadores especiales $? $$ $! -> tratar?
			if (expand->key && expand->key[0] == '?')
				ft_printf("TODO caso especial $?\n");		

			/// EXTRAER VARIABLE  (similar a word_tokenizer)------------------------------
			index++;  // saltar operador $
			expand->first_index = index;
			expand->key = extract_variable(token->final_token, index);
			if (!expand->key)
				return ;
			expand->last_index = index + ft_strlen(expand->key);			

			/// BUSCAR VARIABLE EN ENV ------------------------------
			expand->value = get_environment_var(env,  expand->key);
			if (!expand->value)
				return ; 

			/// SUSTITUIR VARIABLE POR SU VALOR --------------------
			token->final_token = insert_expand_value(token->final_token, expand);

			// verificacion
			print_expand_stack(expand);
			ft_printf("token->final_token -> %s\n", token->final_token);
		}
		index++;
	}	
	// liberacion expand
	free_expand_stack(expand);
} */

char	*insert_expand_value(char *token, t_expand *expand)
{
	char	*expanded_token;
	int		index;
	int		index_token;
	int		index_expand;

	if (!token || !expand)
		return (NULL);
	// reserva total
	expanded_token = (char *) malloc(sizeof(char) * (ft_strlen(token) - (expand->last_index - expand->first_index) + (ft_strlen(expand->value) + 1)));
	index = 0;
	index_token = 0;
	index_expand = 0;
	//ft_printf("expand_var->first_index -> %d\n", expand->first_index);

	// copia token hasta $
	while (index < (expand->first_index - 1))
	{
		expanded_token[index] = token[index_token];
		index++;
		index_token++;
	}
	// copia valor expandido	
	while (expand->value[index_expand])
	{
		expanded_token[index] = expand->value[index_expand];
		index++;
		index_expand++;
	}
	index_token = expand->last_index;
	// copia resto token
	while (token[index_token])
	{
		expanded_token[index] = token[index_token];
		index++;
		index_token++;
	}	
	expanded_token[index] = '\0'; // terminador nulo
	return (expanded_token);
}

char	*extract_variable(char *token, int first_index)
{
	char	*variable;
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
	variable = ft_substr(token, first_index, (index - first_index));
	//ft_printf("variable -> %s\n", variable);
	return (variable);
}

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

t_expand *initialize_expand_stack(void)
{
	t_expand	*expand_stack;

	expand_stack = (t_expand *) malloc(sizeof(t_expand));
	if(!expand_stack)
		return (NULL);	
	
	expand_stack->type	= 0;
	expand_stack->first_index	= -1;  	/// posicion de inicio -1 -> error si no se da valor > 0 ??
	expand_stack->last_index 	= -1;	/// posicion final -1 ??
	expand_stack->substitution_str = NULL;  // variable a sustituir en token
	expand_stack->key	= NULL; // variable extraida
	expand_stack->value	= NULL;	

	expand_stack->next	= NULL;	 
	return (expand_stack);
}
