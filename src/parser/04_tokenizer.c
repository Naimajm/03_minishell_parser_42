/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 14:10:01 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		word_tokenizer(t_word *word);
int		noquotes_tokenizer(t_word *word, int start_index);
int		quotes_tokenizer(t_word *word, int start_index);
int		operator_tokenizer(t_word *word, int start_index);

void	tokenizer(t_word *words_list, t_shell *shell)
{
	t_word	*current_word;
	
	if (!words_list)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));			
	current_word = (t_word *) words_list;
	while (current_word)
	{
		if (word_tokenizer(current_word) == FAILURE)
			shell->exit_status = ERROR;		
		current_word = current_word->next;
	}		
}

int	word_tokenizer(t_word *word)
{
	char	*raw_word;
	int		index;
	int		token_len;

	if (!word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	 	
	raw_word = word->raw_word;		
	index = 0;	
	while (raw_word[index])
	{
		while (is_space(raw_word[index])) 			// ignorar espacios iniciales 
			index++;
		if (!raw_word[index])                       // Verificar si llegamos al final después de saltar espacios
			break;
			
		// CLASIFICACION TOKENS
		if (is_quote(raw_word[index])) 				    // '\'' -> LITERAL // '\"' -> EXPANSION VARIABLE
			token_len = quotes_tokenizer(word, index);
		else if (is_operator(raw_word[index]))
			token_len = operator_tokenizer(word, index);		
		else 											// 1º letra palabra simple sin inicio comillas
			token_len = noquotes_tokenizer(word, index);			
		if (token_len == FAILURE)
			return (FAILURE);
		
		index = advance_index_by_length(index, token_len);  // GESTION CASOS ESPECIALES AVANCE INDEX
		
	}	
	return (SUCCESS);
}

int	noquotes_tokenizer(t_word *word, int start_index)
{
	char	*raw_word;
	char	*token_input;
	int		index;
	int		len_input;

	if (!word || !word->raw_word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	raw_word 	= (char *) word->raw_word;
	index 		= start_index;
	len_input 	= 0;	

	// CASO VARIABLES EXPANDIDAS
	// CASO ESPECIAL: Si empezamos con $, procesamos solo esa variable
    if (raw_word[index] == '$')
    {
        index++;                        // Saltar el $                
        
        if (raw_word[index] == '?')     // Caso $?
            index++;

        // Caso variable normal: $VARIABLE
        else if (ft_isalpha(raw_word[index]) || raw_word[index] == '_')
        {
            // Extraer solo el nombre de la variable
            while ((ft_isalnum(raw_word[index]) || raw_word[index] == '_') 
                && raw_word[index])
                index++;
        }
        // Si $ está seguido de algo inválido, solo tomar el $
        // (el índice ya está en la posición correcta)
    }
    else
    {
        // Texto normal: parar en delimitadores O en $
        while (!is_space(raw_word[index]) 
            && !is_operator(raw_word[index]) 
            && !is_quote(raw_word[index]) 
            && raw_word[index] != '$'           // ¡CLAVE! Parar en $
            && raw_word[index])
            index++;
    }	
	len_input = index - start_index;
	token_input = ft_substr(raw_word, start_index, (index - start_index));
	if (!token_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
		
	add_token_node(&word->tokens_list, token_input, NO_QUOTES);			
	free(token_input);
	return (len_input);
}

int	quotes_tokenizer(t_word *word, int start_index)
{
	char	*raw_word;
	char	*token_input;
	char	delimiter;	
	int		index;	
	int		token_type;
	int		len_input;

	raw_word 		= (char *) word->raw_word;
	delimiter		= raw_word[start_index];	// Comilla de apertura
	index 			= start_index + 1;  		// Empezar después de la comilla
	token_type 		= 0;
	len_input 		= 0;
	
	while (raw_word[index] && raw_word[index] != delimiter)		// Buscar comilla de cierre
		index++;

	if (raw_word[index] == delimiter)							// validacion falta de comilla de cierre
		index++;
	else		
		return (ft_putendl_fd(ERROR_QUOTE_SYNTAX, STDERR_FILENO), FAILURE);
										
	token_input = ft_substr(raw_word, start_index, (index - start_index)); // copia expresion con comillas incluidas
	if (!token_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);	

	// Determinar tipo de token
	if (delimiter == '\"')
		token_type = DOUBLE_QUOTES;
	else if (delimiter == '\'')
		token_type = SINGLE_QUOTES;
	else
		token_type = NO_QUOTES;

	len_input =  ft_strlen(token_input);
	add_token_node(&word->tokens_list, token_input, token_type);
	free(token_input);
	return (len_input);
}

int	operator_tokenizer(t_word *word, int start_index)
{
	char	*raw_word;
	int		len_input;

	if (!word || !word->raw_word)
        return (FAILURE);
	raw_word = (char *) word->raw_word;

	if (raw_word[start_index] == '>') 		
	{
		if (raw_word[start_index + 1] == '>')
		{
			add_token_node(&word->tokens_list, ">>", OPERATOR);
			return (2);
		}				
		else
			add_token_node(&word->tokens_list, ">", OPERATOR);		
	}		
	else if (raw_word[start_index] == '<') 	
	{
		if (raw_word[start_index + 1] == '<')
		{
			add_token_node(&word->tokens_list, "<<", OPERATOR);
			return (len_input = 2);
		}			
		else
			add_token_node(&word->tokens_list, "<", OPERATOR);							
	}		
	else if (is_pipe(raw_word[start_index])) 	    
		add_token_node(&word->tokens_list, "|", OPERATOR);	
	return (len_input = 1);
}

