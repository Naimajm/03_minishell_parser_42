/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01.1_check_syntax.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 23:10:07 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/25 10:37:32 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_pipe_syntax(char *input);
int	check_balanced_quotes(char *input);
int	check_redirection_syntax(char *input);

void	handle_quote_state(bool *inside_quotes, char *quote_char, char current_char);

static bool	is_redirection_char(char c);
static int	validate_redirection_at_position(char *input, int pos);
static int	get_operator_length(char *input, int pos);


// VALIDACION SINTAXIS RAW_INPUT

/* int validate_syntax(t_command *commands)
{
	// Verificar:
	// - No pipes al inicio/final
	// - No operadores consecutivos
	// - Argumentos después de redirects
	// - Comillas balanceadas (ya hecho)
	return (SUCCESS);
} */

char *validate_syntax(t_shell *shell)
{
	char	*message;

	if (!shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	
	message = NULL;
	// VALIDACIONES BASICAS SINTAXIS
	if (check_balanced_quotes(shell->input) == SYNTAX_ERROR)
	{
		shell->exit_status = SYNTAX_ERROR;
		ft_strlcpy(message, ERROR_QUOTE_SYNTAX, ft_strlen(ERROR_QUOTE_SYNTAX));
		return (message);
	}
	if (check_redirection_syntax(shell->input) == SYNTAX_ERROR)
	{
		shell->exit_status = SYNTAX_ERROR;
		ft_strlcpy(message, ERROR_QUOTE_SYNTAX, ft_strlen(ERROR_QUOTE_SYNTAX));
		return (message);
	}
	if (check_pipe_syntax(shell->input) == SYNTAX_ERROR)
	{
		shell->exit_status = SYNTAX_ERROR;
		ft_strlcpy(message, ERROR_QUOTE_SYNTAX, ft_strlen(ERROR_QUOTE_SYNTAX));
		return (message);
	}
	return (message);
}

int	check_balanced_quotes(char *input)
{
    int		i;
    bool	inside_quotes;
    char	quote_char;

    if (!input)
        return (SUCCESS);

    i = 0;
    inside_quotes = false;
    quote_char = 0;

    while (input[i])
    {
        handle_quote_state(&inside_quotes, &quote_char, input[i]);
        i++;
    }

    // Si terminamos y aún estamos dentro de comillas, error
    if (inside_quotes)
        return (SYNTAX_ERROR);

    return (SUCCESS);
}

int	check_redirection_syntax(char *input)
{
    int		i;
    bool	inside_quotes;
    char	quote_char;

    if (!input)
        return (SUCCESS);

    i = 0;
    inside_quotes = false;
    quote_char = 0;

    while (input[i])
    {
        handle_quote_state(&inside_quotes, &quote_char, input[i]);

        if (!inside_quotes && is_redirection_char(input[i]))
        {
            if (validate_redirection_at_position(input, i) == SYNTAX_ERROR)
                return (SYNTAX_ERROR);
        }
        i++;
    }

    return (SUCCESS);
}

int	check_pipe_syntax(char *input)
{
    int		i;
    bool	inside_quotes;
    char	quote_char;

    i = 0;
    inside_quotes = false;
    quote_char = 0;
    
    // Saltar espacios iniciales
    while (input[i] && is_space(input[i]))
        i++;
        
    // Error: pipe al inicio
    if (input[i] == '|')
        return (SYNTAX_ERROR);
    
    while (input[i])
    {
        handle_quote_state(&inside_quotes, &quote_char, input[i]);
        
        if (!inside_quotes && input[i] == '|')
        {
            // Error: pipes consecutivos
            if (input[i + 1] == '|')
                return (SYNTAX_ERROR);
                
            // Error: pipe al final
            int j = i + 1;
            while (input[j] && is_space(input[j]))
                j++;
            if (!input[j])
                return (SYNTAX_ERROR);
        }
        i++;
    }
    return (SUCCESS);
}

void	handle_quote_state(bool *inside_quotes, char *quote_char, char current_char)
{
    if (!inside_quotes || !quote_char)
        return ;

    // Si no estamos dentro de comillas y encontramos una comilla
    if (!(*inside_quotes) && (current_char == '\'' || current_char == '\"'))
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

static bool	is_redirection_char(char c)
{
    return (c == '<' || c == '>');
}

static int	validate_redirection_at_position(char *input, int pos)
{
    int		next_pos;
    //char	*operators[] = {">>", "<<", ">", "<", NULL};
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
    if (is_redirection_char(input[next_pos]) || input[next_pos] == '|')
        return (SYNTAX_ERROR);

    return (SUCCESS);
}
static int	get_operator_length(char *input, int pos)
{
    if (input[pos] == '>' && input[pos + 1] == '>')
        return (2); // >>
    if (input[pos] == '<' && input[pos + 1] == '<')
        return (2); // <<
    if (input[pos] == '>' || input[pos] == '<')
        return (1); // > o <
    
    return (0);
}