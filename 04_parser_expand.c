/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/16 12:31:54 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"


void	activate_expand_operators(t_token *token_list, char **env);
void	expand_token(t_token *token_list, char **env);
char	*insert_expand_value(char *token, t_expand *expand_var);
char	*extract_variable(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
t_expand *initialize_expand_stack(void);

int	search_expand_operators(t_token *token_list)
{
	t_token	*temp_list;
	int	match;

	if (!token_list)
		return (0);
	temp_list = (t_token *) token_list;
	match = 0;
	while (temp_list)
	{
		if (ft_strchr(temp_list->raw_token, '$'))
		{
			match = 1;
			temp_list->expand_var = true;   // activar funcion expansion en ese token			
		}			
		temp_list = temp_list->next;
	}
	return (match);	
}

void	activate_expand_operators(t_token *token_list, char **env)
{
	t_token	*ptr_list;

	if (!token_list)
		return ;
	ptr_list = (t_token *) token_list;

	while (ptr_list)
	{
		if (ptr_list->expand_var)		
			expand_token(ptr_list, env);
		ptr_list = ptr_list->next;
	}	
}

void	expand_token(t_token *token, char **env)
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
}

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
	variable = NULL;
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
		
	expand_stack->first_index	= -1;  	/// posicion de inicio -1 -> error si no se da valor > 0 ??
	expand_stack->last_index 	= -1;	/// posicion final -1 ??
	expand_stack->key	= NULL; // nombre variable extraida
	expand_stack->value	= NULL; // valor de la variable
	return (expand_stack);
}
