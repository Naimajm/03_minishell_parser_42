/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/23 16:19:43 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void generate_expand_list(t_token *token); 
void resolve_key_values(t_token *token, t_shell *shell);

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
		resolve_key_values(current_token, shell);

		// 2. RESOLVER LOS VALORES PARA CADA NODO DE EXPANSIÓN
			// BUSCAR VALUE -
			// shell->env
			// shell->exit-status


		// PREFIJO +  INSERTAR VALUE - QUITAR SUBSTITUTION_VAR + SUFIJO
		current_token = current_token->next;
	}	
	//printf("final activate_expand_operators\n");
}

void generate_expand_list(t_token *token)
{
	int		index;
	int		subs_len;

	if (!token)
		return ; 	
	index 		= 0;		
	while (token->raw_token[index]) // CATEGORIZACION EXPAND NODOS
	{
		if (token->raw_token[index] == '$' && token->raw_token[index + 1])
		{
			if (token->raw_token[index + 1] == '?') // GESTION CASO ESPECIAL "$?
				subs_len = last_exit_status_expander(token, index);			
			else if (index > 0 && token->raw_token[index - 1] == '\\') // caso \$VAR -> literal , no inicia expancion
				subs_len = literal_expander(token, index);
			else if (token->raw_token[index + 1] == '{') // caso ${VAR}xx
				subs_len = curly_braces_expander(token, index);
			else // caso normal -> EXPANSION BASICA -> AÑADIR A LISTA EXPAND
				subs_len = basic_expander(token, index);				
			if (subs_len == FAILURE)	// error
				return ;			
			index += subs_len;	
		}	
		else	
            index++;	 // $sin caracter adicional o resto caracteres
	}
}

void resolve_key_values(t_token *token, t_shell *shell)
{
	t_expand	*expand_node;
	char		*key;

	if (!token || !shell)
		return ;
	//key = NULL;
	//(void)shell; // silenciar
	expand_node = (t_expand *)(token->expand_list);
	while (expand_node)
	{		
		// determinar key segun tipo de expansion
		if (expand_node->type == LAST_EXIT_STATUS)
		{
			//printf ("proceso %i\n", LAST_EXIT_STATUS);
			key = ft_strdup("$?");
			expand_node->key = key;
			expand_node->value = ft_itoa(shell->exit_status);
		}			
		else if (expand_node->type == CURLY_BRACES)
		{
			key = extract_key(token->expand_list->substitution_str, 1); // index 0 -> $
			key = ft_strtrim(key, "{}");  // cortar {}		

			expand_node->key = key;
			expand_node->value = get_environment_var(shell->environment, expand_node->key);
			//printf ("key -> %s\n", key);		
			//printf ("proceso %i\n", CURLY_BRACES);
		}				
		else if (expand_node->type == LITERAL)
		{
			//printf ("proceso %i\n", LITERAL);
			key = ft_strdup(expand_node->substitution_str);
			expand_node->key = key;
			expand_node->value = ft_strdup(key);
		}		
		else
		{
			key = extract_key(expand_node->substitution_str, 1); // index 0 -> $			
			expand_node->key = key;		
			expand_node->value = get_environment_var(shell->environment, expand_node->key);
			//printf ("proceso %i -> key %s\n", BASIC_EXPANSION, key);
		}	
		expand_node = expand_node->next;
	}
}






