/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_parser_expand_extract.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/22 18:36:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// FUNCIONES AUXILIARES EXTRACCION SUBSTITUTION STRING


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


