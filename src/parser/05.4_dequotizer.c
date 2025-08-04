/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05.4_dequotizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:46:05 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/04 12:39:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// FUNCIONES PARA QUITAR COMILLAS + UNIR TOKENS
static int	remove_quotes(t_token *tokens_list);
void update_quote_state(char character, bool *in_single_quotes, bool *in_double_quotes);

void	dequotize_tokens(t_word *words_list, t_shell *shell)
{
	t_word	*current_word;

	if (!words_list || !shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_word = (t_word *) words_list;
	while (current_word)
	{
		if (remove_quotes(current_word->tokens_list) == FAILURE)
			shell->exit_status = ERROR;
		current_word = current_word->next;
	}
}

// funcion para quitar comillas de 'expanded_token' a 'final_token'
static int	remove_quotes(t_token *tokens_list)
{
	t_token *current_token;

	if (!tokens_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	current_token = (t_token *) tokens_list;
	while (current_token)
	{
		if (current_token->type == DOUBLE_QUOTES 
			|| current_token->type == SINGLE_QUOTES)
		{
			current_token->noquotes_token = ft_substr(current_token->expanded_token, 1, ft_strlen(current_token->expanded_token) - 2); 
			if (!current_token->noquotes_token)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
		}
		else
		{
			current_token->noquotes_token = ft_strdup(current_token->expanded_token);	
			if (!current_token->noquotes_token)
				return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
		}	
		current_token = current_token->next;
	}
	return (SUCCESS);	
}

// FUNCION AUXILIAR PARA MANEJAR EL ESTADO DE COMILLAS SIMPLES O DOBLES
void update_quote_state(char character, bool *in_single_quotes, bool *in_double_quotes)
{
    if (character == '\'' && *in_double_quotes == false)
    {
        if (*in_single_quotes == false)
            *in_single_quotes = true;      // Entrar en comillas simples
        else
            *in_single_quotes = false;     // Salir de comillas simples
    }
    else if (character == '"' && *in_single_quotes == false)
    {
        if (*in_double_quotes == false)
            *in_double_quotes = true;      // Entrar en comillas dobles
        else
            *in_double_quotes = false;     // Salir de comillas dobles
    }
}

