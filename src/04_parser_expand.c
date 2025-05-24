/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/24 12:08:44 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void generate_expand_list(t_token *token); 
void resolve_expansion_values(t_token *token, t_shell *shell);

void	activate_expand_operators(t_shell *shell)
{
	t_token	*current_token;

	if (!shell->token_list)
		return ;
	current_token = (t_token *) shell->token_list;

	while (current_token)
	{
		if (current_token->type == SINGLE_QUOTES) // no hay expansion $
		{
			current_token = current_token->next;			
			continue;
		}  

		// GENERAR LISTAS NODOS EXPAND, KEY, VALUE
		generate_expand_list(current_token);

		// resolver valores
		resolve_expansion_values(current_token, shell);

		// PREFIJO +  INSERTAR VALUE - QUITAR SUBSTITUTION_VAR + SUFIJO

		// IMPRIMIR NODOS EXPAND PARA DEPURAR
        //print_expand_list(current_token->expand_list);
		
		current_token = current_token->next;
	}	
	printf("final activate_expand_operators\n");
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

void resolve_expansion_values(t_token *token, t_shell *shell)
{
	t_expand	*expand_node;
	char		*value;

	if (!token || !shell)
		return ;
	expand_node = (t_expand *)(token->expand_list);
	while (expand_node)
	{
		//value = NULL;
		if (expand_node->type == LAST_EXIT_STATUS)
			value = ft_itoa(shell->exit_status);		
		else if (expand_node->type == CURLY_BRACES)
			value = get_environment_var(shell->environment, expand_node->key);			
		else if (expand_node->type == LITERAL)
			value = ft_strdup(expand_node->key);
		else
			value = get_environment_var(shell->environment, expand_node->key);
		expand_node->value = value;
		expand_node = expand_node->next;
	}
}






