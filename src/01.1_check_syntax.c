/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01.1_check_syntax.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:10:07 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/30 13:29:00 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			check_pipe_syntax(char *input);
int			check_balanced_quotes(char *input);
int			check_redirection_syntax(char *input);
void		handle_quote_state(bool *inside_quotes, char *quote_char, char current_char);

static bool	is_redirection_char(char c);
static int	validate_redirection_at_position(char *input, int pos);
static int	get_operator_length(char *input, int pos);

int	validate_syntax(t_shell *shell)
{
	if (!shell->input)
		return (SYNTAX_ERROR);	

	// VALIDACIONES BASICAS SINTAXIS
	if (check_pipe_syntax(shell->input) == SYNTAX_ERROR)
	{
		ft_putendl_fd(ERROR_PIPE_SYNTAX, STDERR_FILENO);
        shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	if (check_balanced_quotes(shell->input) == SYNTAX_ERROR)
	{
		ft_putendl_fd(ERROR_QUOTE_SYNTAX, STDERR_FILENO);
        shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	if (check_redirection_syntax(shell->input) == SYNTAX_ERROR)
	{
		ft_putendl_fd(ERROR_REDIRECTION_SYNTAX, STDERR_FILENO);
        shell->exit_status = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int check_pipe_syntax(char *input)
{
    int     index;
    bool    inside_quotes;
    char    quote_char;

    bool    found_pipe;
    bool    found_command_after_pipe;
    int     temp_index;
    
    index           = 0;
    inside_quotes   = false;
    quote_char      = 0;
    found_pipe      = false;
    found_command_after_pipe = false;
    temp_index      = 0;    
    
    while (input[index] && is_space(input[index]))      // Saltar espacios iniciales
        index++;        
    
    if (is_pipe(input[index]))                            // ERROR: Pipe al inicio
        return (SYNTAX_ERROR);
    
    while (input[index])
    {
        handle_quote_state(&inside_quotes, &quote_char, input[index]);
        
        if (!inside_quotes && is_pipe(input[index]))
        {
            // Si ya encontramos un pipe y no hubo comando válido después
            if (found_pipe && !found_command_after_pipe)
                return (SYNTAX_ERROR);                  // Comando vacío entre pipes
                
            found_pipe = true;
            found_command_after_pipe = false;            
            
            if (input[index + 1] == '|')                // Verificar double pipe
                return (SYNTAX_ERROR);                
            
            temp_index = index + 1;                     // Saltar espacios después del pipe
            while (input[temp_index] && is_space(input[temp_index]))
                temp_index++;                
            
            if (!input[temp_index] || is_pipe(input[temp_index]))   // ERROR: Pipe al final o seguido de otro pipe
                return (SYNTAX_ERROR);
        }
        else if (!inside_quotes && !is_space(input[index]) && is_pipe(input[index]))
        {            
            if (found_pipe)                         // Encontramos contenido real después de un pipe
                found_command_after_pipe = true;
        }        
        index++;
    }    
    return (SUCCESS);
}

int	check_balanced_quotes(char *input)
{
    int		index;
    bool	inside_quotes;
    char	quote_char;

    if (!input)
        return (SUCCESS);
    index           = 0;
    inside_quotes   = false;
    quote_char      = 0;

    while (input[index])
    {
        handle_quote_state(&inside_quotes, &quote_char, input[index]);
        index++;
    }    
    if (inside_quotes)              // Si terminamos y aún estamos dentro de comillas, error
        return (SYNTAX_ERROR);

    return (SUCCESS);
}

int	check_redirection_syntax(char *input)
{
    int		index;
    bool	inside_quotes;
    char	quote_char;

    if (!input)
        return (SUCCESS);
    index           = 0;
    inside_quotes   = false;
    quote_char      = 0;

    while (input[index])
    {
        handle_quote_state(&inside_quotes, &quote_char, input[index]);

        if (!inside_quotes && is_redirection_char(input[index]))
        {
            if (validate_redirection_at_position(input, index) == SYNTAX_ERROR)
                return (SYNTAX_ERROR);
        }
        index++;
    }
    return (SUCCESS);
}

void	handle_quote_state(bool *inside_quotes, char *quote_char, char current_char)
{
    if (!inside_quotes || !quote_char)
        return ;

    // Si no estamos dentro de comillas y encontramos una comilla
    if (!(*inside_quotes) && is_quote(current_char))
    {
        *inside_quotes = true;
        *quote_char = current_char;
    }
    // Si estamos dentro de comillas y encontramos la comilla de cierre
    else if (*inside_quotes && current_char == *quote_char)
    {
        *inside_quotes = false;
        *quote_char = 0;
    }
    // Si estamos dentro de comillas simples, todos los caracteres son literales
    // Si estamos dentro de comillas dobles, solo la comilla doble cierra
}

static int	validate_redirection_at_position(char *input, int pos)
{
    int		next_pos;
    int		op_len;

    // Determinar qué operador tenemos
    op_len = get_operator_length(input, pos);
    if (op_len == 0)
        return (SYNTAX_ERROR);

    next_pos = pos + op_len;

    // Saltar espacios después del operador
    while (input[next_pos] && is_space(input[next_pos]))
        next_pos++;

    // Error: no hay archivo después del operador
    if (!input[next_pos])
        return (SYNTAX_ERROR);

    // Error: otro operador inmediatamente después
    if (is_redirection_char(input[next_pos]) || is_pipe(input[next_pos]))
        return (SYNTAX_ERROR);

    return (SUCCESS);
}

static bool	is_redirection_char(char c)
{
    return (c == '<' || c == '>');
}
static int	get_operator_length(char *input, int index)
{
    if (input[index] == '>' && input[index + 1] == '>')
        return (2); // >>
    if (input[index] == '<' && input[index + 1] == '<')
        return (2); // <<
    if (input[index] == '>' || input[index] == '<')
        return (1); // > o <    
    return (0);
}