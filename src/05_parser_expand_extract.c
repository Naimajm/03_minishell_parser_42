/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_parser_expand_extract.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/24 12:19:01 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES AUXILIARES EXTRACCION SUBSTITUTION STRING
int		basic_expander(t_token *token, int first_index);
int		last_exit_status_expander(t_token *token, int first_index);
int		curly_braces_expander(t_token *token, int first_index);
int		literal_expander(t_token *token, int first_index);
char	*extract_substitution_segment(char *raw_token, int first_index);

int	basic_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;
	char		*key;

	if (!token)
		return (FAILURE);
	len_input = 0;		
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		return (FAILURE);	
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expand_list, substitution_str, first_index, BASIC_EXPANSION);

	key = extract_key(substitution_str, 1); // index 0 -> $	
	expand_node->key = key;

	//value = get_environment_var(shell->environment, token->expand_list->key);	
	//set_expand_key_value(token->expand_list, key, value);	
	free(substitution_str);	
	return (len_input);	
}

int	last_exit_status_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char	*substitution_str;
	int		len_input;
	char	*key;

	if (!token)
		return (FAILURE);
	substitution_str = ft_strdup("$?"); // copiar sub substr
	if (!substitution_str)
		return (FAILURE);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expand_list, substitution_str, first_index, LAST_EXIT_STATUS);	

	key = ft_strdup("$?");
	expand_node->key = key;

	//value = ft_itoa(shell->exit_status);
	//set_expand_key_value(expand_node, key, value);		
	free(substitution_str);
	//ft_printf("token -> %s\n", token);
	return (len_input);
}

int	curly_braces_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			final_index;
	int			len_input;
	char		*key;

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
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expand_list, substitution_str, first_index, CURLY_BRACES);
	
	key = extract_key(substitution_str, 1); // index 0 -> $
	key = ft_strtrim(key, "{}");  // cortar {}
	expand_node->key = key;
	
	//value = get_environment_var(shell->environment, expand_node->key);
	//set_expand_key_value(expand_node, key, value);	
	free(substitution_str);	
	return (len_input); //ft_printf("quote_token -> %s\n", token);	
}

int	literal_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;
	char		*key;

	if (!token)
		return (FAILURE);	
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		return (FAILURE);
	//ft_printf("substitution_str -> %s\n", substitution_str);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expand_list, substitution_str, first_index, LITERAL);		

	key = ft_strdup(substitution_str);
	expand_node->key = key;
	
	//value = ft_strdup(key);
	//set_expand_key_value(expand_node, key, value);
	free(substitution_str);	
	return (len_input);
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


