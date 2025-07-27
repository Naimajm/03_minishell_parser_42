/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:25 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/27 13:42:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	word_tokenizer(t_word *word);
int		noquotes_tokenizer(t_word *word, int start_index);
int		quotes_tokenizer(t_word *word, int start_index);
int		operator_tokenizer(t_word *word, int start_index);

//static int	process_double_quotes_content(t_word *word, int start_index, int end_index);

void	tokenizer(t_word *words_list)
{
	t_word	*current_word;
	printf("DEBUG tokenizer\n");
	if (!words_list)
	{
		ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO);
		return ;
	}
			
	current_word = (t_word *) words_list;
	while (current_word)
	{
		word_tokenizer(current_word);			
		current_word = current_word->next;
	}		
}

void	word_tokenizer(t_word *word)
{
	char	*raw_word;
	int		index;
	int		token_len;

	if (!word)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));	 	
	raw_word = word->raw_word;		
	index = 0;	
	while (raw_word[index])
	{
		while (is_space(raw_word[index])) 			// ignorar espacios iniciales 
			index++;
		if (!raw_word[index]) // Verificar si llegamos al final después de saltar espacios
			break;
			
		// CLASIFICACION TOKENS
		if (is_quote(raw_word[index])) 				// '\'' -> LITERAL // '\"' -> EXPANSION VARIABLE
			token_len = quotes_tokenizer(word, index);
		else if (is_operator(raw_word[index]))
			token_len = operator_tokenizer(word, index);		
		else 											// 1º letra palabra simple sin inicio comillas
			token_len = noquotes_tokenizer(word, index);

		// GESTION CASOS ESPECIALES AVANCE INDEX
		index = advance_index_by_length(index, token_len);
	}	
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
        index++; // Saltar el $
        
        // Caso $?
        if (raw_word[index] == '?')
        {
            index++;
        }
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
            && raw_word[index] != '$'  // ¡CLAVE! Parar en $
            && raw_word[index])
            index++;
    }
	
	/* while (!is_space(raw_word[index]) // longitud de caracteres de la palabra -> limites > < | " " '"' /0
		&& !is_operator(raw_word[index]) 
		&& !is_quote(raw_word[index]) 
		&& raw_word[index])
		index++;	 */
	
	len_input = index - start_index;
	token_input = ft_substr(raw_word, start_index, (index - start_index)); // copiar sub substr
	if (!token_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);
		
	//len_input = ft_strlen(token_input);
	add_token_node(&word->tokens_list, token_input, NO_QUOTES);			
	free(token_input);
	//ft_printf("token -> %s\n", token);
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

	// Buscar comilla de cierre
	while (raw_word[index] && raw_word[index] != delimiter)
		index++;

	if (raw_word[index] == delimiter)
		index++;
	else					// validacion falta de comilla de cierre
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

	raw_word = (char *) word->raw_word;

	if (raw_word[start_index] == '>') 			// operadores especiales -> OUTFILE o APPEND
	{
		if (raw_word[start_index + 1] == '>')
		{
			add_token_node(&word->tokens_list, ">>", OPERATOR);
			return (len_input = 2);
		}				
		else
			add_token_node(&word->tokens_list, ">", OPERATOR);		
	}		
	else if (raw_word[start_index] == '<') 	// operadores especiales -> INFILE o HERE_DOC
	{
		if (raw_word[start_index + 1] == '<')
		{
			add_token_node(&word->tokens_list, "<<", OPERATOR);
			return (len_input = 2);
		}			
		else
			add_token_node(&word->tokens_list, "<", OPERATOR);							
	}		
	else if (raw_word[start_index] == '|') 	// operadores especiales -> PIPE
		add_token_node(&word->tokens_list, "|", OPERATOR);	
	return (len_input = 1);
}
/* 
static int	process_double_quotes_content(t_word *word, int start_index, int end_index)
{
    char	*raw_word;
    char	*content;
    int		content_index;
    int		segment_start;
    int		total_len;

    raw_word = word->raw_word;
    total_len = end_index - start_index;
    
    // Agregar comilla de apertura como token separado
    add_token_node(&word->tokens_list, "\"", DOUBLE_QUOTES);
    
    // Procesar contenido entre comillas
    content_index = start_index + 1; // Saltar comilla de apertura

	printf("DEBUG: processing quotes content from %d to %d\n", start_index, end_index);
    printf("DEBUG: raw_word = '%s'\n", raw_word);
    
    while (content_index < end_index - 1) // Hasta antes de comilla de cierre
    {
		printf("DEBUG: content_index = %d, char = '%c'\n", content_index, raw_word[content_index]);
        segment_start = content_index;
        
        // Si encontramos $, procesamos la variable
        if (raw_word[content_index] == '$')
        {
            content_index++; // Saltar $
            
            // Verificar que no estamos al final
            if (content_index >= end_index - 1)
            {
                // Solo queda el $, crear token para él
                content = ft_substr(raw_word, segment_start, 1);
                if (content)
                {
                    add_token_node(&word->tokens_list, content, DOUBLE_QUOTES);
                    free(content);
                }
                break;
            }
            
            // Caso $?
            if (raw_word[content_index] == '?')
            {
                content_index++;
            }
            // Variable normal
            else if (ft_isalpha(raw_word[content_index]) || raw_word[content_index] == '_')
            {
                while ((ft_isalnum(raw_word[content_index]) || raw_word[content_index] == '_') 
                    && content_index < end_index - 1)
                    content_index++;
            }
            // Si $ está seguido de carácter inválido para variable, solo el $
            // content_index ya está en la posición correcta
        }
        else
        {
            // Texto literal hasta encontrar $ o fin de comillas
            while (raw_word[content_index] != '$' && content_index < end_index - 1)
                content_index++;
        }
        
        // Crear token para este segmento si hay contenido
        if (content_index > segment_start)
        {
            content = ft_substr(raw_word, segment_start, content_index - segment_start);
            if (!content)
                return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
            
            add_token_node(&word->tokens_list, content, DOUBLE_QUOTES);
            free(content);
        }
        
        // IMPORTANTE: Asegurar que avanzamos para evitar bucle infinito
        if (content_index == segment_start)
        {
            // Si no hemos avanzado, forzar avance de un carácter
            content_index++;
        }
    }
    
    // Agregar comilla de cierre como token separado
    add_token_node(&word->tokens_list, "\"", DOUBLE_QUOTES);
    
    return (total_len);
}
 */
/* static int	process_double_quotes_content(t_word *word, int start_index, int end_index)
{
    char	*raw_word;
    char	*content;
    int		content_index;
    int		segment_start;
    int		total_len;

    raw_word = word->raw_word;
    total_len = end_index - start_index;
    
    // Agregar comilla de apertura como token separado
    add_token_node(&word->tokens_list, "\"", DOUBLE_QUOTES);
    
    // Procesar contenido entre comillas
    content_index = start_index + 1; // Saltar comilla de apertura
    
    while (content_index < end_index - 1) // Hasta antes de comilla de cierre
    {
        segment_start = content_index;
        
        // Si encontramos $, procesamos la variable
        if (raw_word[content_index] == '$')
        {
            content_index++; // Saltar $
            
            // Caso $?
            if (raw_word[content_index] == '?')
            {
                content_index++;
            }
            // Variable normal
            else if (ft_isalpha(raw_word[content_index]) || raw_word[content_index] == '_')
            {
                while ((ft_isalnum(raw_word[content_index]) || raw_word[content_index] == '_') 
                    && content_index < end_index - 1)
                    content_index++;
            }
            // $ seguido de carácter inválido - solo tomar el $
        }
        else
        {
            // Texto literal hasta encontrar $ o fin de comillas
            while (raw_word[content_index] != '$' && content_index < end_index - 1)
                content_index++;
        }
        
        // Crear token para este segmento
        if (content_index > segment_start)
        {
            content = ft_substr(raw_word, segment_start, content_index - segment_start);
            if (!content)
                return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), FAILURE);
            
            add_token_node(&word->tokens_list, content, DOUBLE_QUOTES);
            free(content);
        }
    }
    
    // Agregar comilla de cierre como token separado
    add_token_node(&word->tokens_list, "\"", DOUBLE_QUOTES);
    
    return (total_len);
} */
