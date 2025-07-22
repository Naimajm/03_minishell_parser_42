/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.1_expand_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/22 11:30:27 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES AUXILIARES EXTRACCION SUBSTITUTION STRING
int		basic_expander(t_token *token, int first_index);
int		last_exit_status_expander(t_token *token, int first_index);
int		curly_braces_expander(t_token *token, int first_index);
int		literal_expander(t_token *token, int first_index);

int	basic_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char		*substitution_str;
	int			len_input;
	char		*key;

	if (!token)
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	len_input = 0;		
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);	
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, BASIC_EXPANSION);

	key = extract_key(substitution_str, 1); // index 0 -> $	
	expand_node->key = key;
	free(substitution_str);	

	//printf("(basic_expander) len_input -> %i\n", len_input);	
	return (len_input);	
}

int	last_exit_status_expander(t_token *token, int first_index)
{
	t_expand	*expand_node;
	char	*substitution_str;
	int		len_input;
	char	*key;

	if (!token)
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	substitution_str = ft_strdup("$?"); // copiar sub substr
	if (!substitution_str)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, LAST_EXIT_STATUS);	

	key = ft_strdup("$?");
	if (!key)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
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
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);	
	final_index = first_index + 2; 		// Saltar "${"
	while (token->raw_token[final_index] && token->raw_token[final_index] != '}')
		final_index++;
	if (token->raw_token[final_index] != '}') // No se encontró llave de cierre
		return (GENERAL_ERROR); 	

	substitution_str = ft_substr(token->raw_token, first_index, (final_index - first_index + 1));
	if (!substitution_str)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);

	len_input = ft_strlen(substitution_str);
	//printf("len_input _> %i\n", len_input);
	
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, CURLY_BRACES);
	
	key = extract_key(substitution_str, 1); 	// index 0 -> $
	key = ft_strtrim(key, "{}");  				// cortar {}
	expand_node->key = key;

	// calcular last_index del nodo expand
	expand_node->last_index = first_index + len_input - 1;
	
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
		print_error(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);	
	substitution_str = extract_substitution_segment(token->raw_token, first_index);
	if (!substitution_str)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	//ft_printf("substitution_str -> %s\n", substitution_str);
	len_input = ft_strlen(substitution_str);
	expand_node = add_expand_node(&token->expands_list, substitution_str, first_index, LITERAL);		

	key = ft_strdup(substitution_str);
	if (!key)
		print_error(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
	expand_node->key = key;
	
	free(substitution_str);	
	return (len_input);
}
