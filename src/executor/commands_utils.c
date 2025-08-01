/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumarque <jumarque@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:02:27 by sbenitez          #+#    #+#             */
/*   Updated: 2025/07/31 18:55:10 by jumarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_count_args(t_token *token)
{
	int	i;

	i = 0;
	while (token && (token->type == 0 || token->type == SINGLE_QUOTES || token->type == INFILE
			|| token->type == OUTFILE || token->type == APPEND || token->type == HERE_DOC))
	{
		if (token->type == INFILE || token->type == OUTFILE || token->type == APPEND
			|| token->type == HERE_DOC)
			token = token->next->next;
		else
		{
			i++;
			token = token->next;
		}
	}
	return (i);
}

int	ft_isbuiltin(char *str)
{
	if (!ft_strcmp(str, "echo"))
		return (1);
	if (!ft_strcmp(str, "cd"))
		return (1);
	else if (!ft_strcmp(str, "pwd"))
		return (1);
	else if (!ft_strcmp(str, "export"))
		return (1);
	else if (!ft_strcmp(str, "unset"))
		return (1);
	else if (!ft_strcmp(str, "env"))
		return (1);
	else if (!ft_strcmp(str, "exit"))
		return (1);
	else
		return (0);
}

char	*ft_substr_malloc(const char *input, int start, int len)
{
	char	*substr;

	if (!input || start < 0 || len < 0)
		return (NULL);
	substr = safe_malloc(sizeof(char) * (len + 1)); // buscar donde está 
	ft_strlcpy(substr, &input[start], len);
	return (substr);
}
