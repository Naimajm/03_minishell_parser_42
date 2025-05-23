/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_parser_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:21:41 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/23 13:45:22 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void generate_expand_list(t_token *token); 

int	basic_expander(t_token *token, int first_index);
int	last_exit_status_expander(t_token *token, int first_index);
int	curly_braces_expander(t_token *token, int first_index);
int	literal_expander(t_token *token, int first_index);

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

int	basic_expander(t_token *token, int first_index)
{
	char	*substitution_str;
	int		len_input;

	if (!token)
		return (FAILURE);
	len_input = 0;		
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		return (FAILURE);	
	len_input = ft_strlen(substitution_str);
	add_expand_node(&token->expand_list, substitution_str, first_index, BASIC_EXPANSION);		
	free(substitution_str);	
	return (len_input);	
}

int	last_exit_status_expander(t_token *token, int first_index)
{
	char	*substitution_str;
	int		len_input;

	if (!token)
		return (FAILURE);
	substitution_str = ft_strdup("$?"); // copiar sub substr
	if (!substitution_str)
		return (FAILURE);
	len_input = ft_strlen(substitution_str);
	add_expand_node(&token->expand_list, substitution_str, first_index, LAST_EXIT_STATUS);			
	free(substitution_str);
	//ft_printf("token -> %s\n", token);
	return (len_input);
}

int	curly_braces_expander(t_token *token, int first_index)
{
	char	*substitution_str;
	int		final_index;
	int		len_input;

	if (!token)
		return (FAILURE);	
	final_index = first_index + 2;
	while (token->raw_token[final_index] && token->raw_token[final_index] != '}')
		final_index++;

	if (token->raw_token[final_index] != '}')
		return (FAILURE); // No se encontró llave de cierre
	substitution_str = ft_substr(token->raw_token, first_index, (final_index - first_index + 1));
	if (!substitution_str)
		return (FAILURE);
	ft_printf("substitution_str -> %s\n", substitution_str);
	len_input = ft_strlen(substitution_str);
	add_expand_node(&token->expand_list, substitution_str, first_index, CURLY_BRACES);	
	free(substitution_str);	
	return (len_input); //ft_printf("quote_token -> %s\n", token);	
}

int	literal_expander(t_token *token, int first_index)
{
	char	*substitution_str;
	int		len_input;

	if (!token)
		return (FAILURE);	
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		return (FAILURE);
	ft_printf("substitution_str -> %s\n", substitution_str);
	len_input = ft_strlen(substitution_str);
	add_expand_node(&token->expand_list, substitution_str, first_index, LITERAL);		
	free(substitution_str);	
	return (len_input);
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


