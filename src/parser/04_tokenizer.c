/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/01 19:23:55 by juagomez         ###   ########.fr       */
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
	
	if (!words_list || !shell)
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
		else if (is_redirection(raw_word[index]) || is_pipe(raw_word[index]))
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
	int		index;
	int		len_input;

	if (!word || !word->raw_word || start_index < 0)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	raw_word 	= (char *) word->raw_word;
	index 		= start_index;

	// DELIMITADOR LIMITES CARACTERES TOKEN -------------------------------------------
	// CASO VARIABLES EXPANDIDAS	Si empezamos con $, procesamos solo esa variable
    if (raw_word[index] == '$')
    {
        index++;                       // Saltar el $        
        if (raw_word[index] == '?')     // Caso $?
            index++;       
        else if (ft_isalpha(raw_word[index]) || raw_word[index] == '_')	 // Caso variable normal: $VARIABLE
        {            
            while ((ft_isalnum(raw_word[index]) || raw_word[index] == '_')  
                && raw_word[index])					// Extraer solo el nombre de la variable
                index++;
        }
    }
    else
    {        
        while (!is_space(raw_word[index]) 			// Texto normal: parar en delimitadores O en $
			&& !is_redirection(raw_word[index])
			&& !is_pipe(raw_word[index])
            && !is_quote(raw_word[index]) 
            && raw_word[index] != '$'           	// ¡CLAVE! Parar en $
            && raw_word[index])
            index++;
    }	
	len_input = index - start_index;
	
	// CREAR NODO TOKEN ------------------------------------------------------
	create_and_add_token(&word->tokens_list, raw_word, start_index, len_input, NO_QUOTES);
	return (len_input);
}

int	quotes_tokenizer(t_word *word, int start_index)
{
	char	*raw_word;	
	int		index;	
	char	delimiter;	
	int		token_type;
	int		len_input;

	raw_word 		= (char *) word->raw_word;	
	index 			= start_index + 1;  		// Empezar después de la comilla
	delimiter		= raw_word[start_index];	// Comilla de apertura
	// CLASIFICAR TIPO TOKEN 
	if (delimiter == '\"')
		token_type = DOUBLE_QUOTES;
	else if (delimiter == '\'')
		token_type = SINGLE_QUOTES;
	else
		token_type = NO_QUOTES;

	// DELIMITADOR LIMITES CARACTERES TOKEN -------------------------------------------
	while (raw_word[index] && raw_word[index] != delimiter)		// Buscar comilla de cierre
		index++;
	if (raw_word[index] == delimiter)							// validacion falta de comilla de cierre
		index++;
	else		
		return (ft_putendl_fd(ERROR_QUOTE_SYNTAX, STDERR_FILENO), FAILURE);	
	len_input =  index - start_index;

	// CREAR NODO TOKEN ------------------------------------------------------
	create_and_add_token(&word->tokens_list, raw_word, start_index, len_input, token_type);
	return (len_input);
}

int	operator_tokenizer(t_word *word, int start_index)
{
	char	*raw_word;
	int		operator_len;
	char	*operator;

	if (!word || start_index < 0)
        return (FAILURE);
	raw_word 		= (char *) word->raw_word;
	operator_len 	= get_operator_length(raw_word, start_index);
	if (operator_len == 0)
        return (FAILURE);
	operator 		= NULL;
    if (operator_len == 2 && raw_word[start_index] == '>')
        operator = ">>";
    else if (operator_len == 2 && raw_word[start_index] == '<')
        operator = "<<";
    else if (operator_len == 1 && raw_word[start_index] == '>')
        operator = ">";
    else if (operator_len == 1 && raw_word[start_index] == '<')
        operator = "<";
    else if (operator_len == 1 && is_pipe(raw_word[start_index]))
        operator = "|";
	else
		return (FAILURE); 
	// CREAR NODO TOKEN ------------------------------------------------------
	create_and_add_token(&word->tokens_list, operator, start_index, operator_len, OPERATOR);
	return (operator_len);
}



