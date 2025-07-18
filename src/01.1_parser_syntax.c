/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01.1_parser_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:01:32 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 11:58:51 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// 	ANALISIS SINTACTICO PARA GENERACION LISTA DE COMANDOS

void	create_commands_structure(t_shell *shell);
int 	generate_command(t_shell *shell, int index_first_char);
int 	is_pipe(char character);

void	syntax_analyzer(t_shell *shell)
{
	if (!shell)
		return ;
		
	// VALIDACION SINTACTICA
	/* if (validate_syntax(words_list) == FAILURE)
	{
		print_message_and_exit(ERROR_SYNTAX, STDERR_FILENO, FAILURE);
		return;
	} */

	// CREAR ESTRUCTURA COMANDOS
	create_commands_structure(shell);	
	
}

/* int validate_syntax(t_command *commands)
{
	// Verificar:
	// - No pipes al inicio/final
	// - No operadores consecutivos
	// - Argumentos después de redirects
	// - Comillas balanceadas (ya hecho)
	return (SUCCESS);
} */

// AGRUPAR INPUT NO PROCESADOS PARA DIFERENTES JOBS EN EL CASO DE PIPES
void	create_commands_structure(t_shell *shell)
{
	int		index;
	int		input_len;

	if (!shell || !shell->input)
		return ; 	
	index = 0;	
	while (shell->input[index])
	{
		while (is_space(shell->input[index])) // ignorar espacios iniciales 
			index++;
		if (!shell->input[index]) 			// Verificar si llegamos al final después de saltar espacios
			break;
		
		// 1º ESTRUCTURA PROCESO

		// CLASIFICACION LISTA PROCESOS
		input_len = generate_command(shell, index);

		if (input_len == 0)	 // caso especial input = 0;
			index++;
		else
			index += input_len;

		// CASO PIPE -> ' | ' (operador aislado con espacios)
		while (is_space(shell->input[index])) // ignorar espacios iniciales 
			index++;

		// Si encontramos un pipe, saltarlo para el siguiente comando
		if (is_pipe(shell->input[index]))
		{
			index++; // Saltar el pipe
			printf("Found pipe, creating new command structure\n");
		}	
	}	
}

int generate_command(t_shell *shell, int index_first_char)
{
	char	*chunk_input;
	int		index;
	int		len_input;
	int		quote_state;
	char	current_quote;

	if (!shell->input)
		return (FAILURE);

	index = index_first_char;
	quote_state = 0;
	current_quote = 0;

	// Procesar hasta encontrar pipe fuera de comillas o final de input
	while (shell->input[index])
	{
		char c = shell->input[index];
		
		// Manejar comillas
		if (is_quote(c))
		{
			if (quote_state == 0)
			{
				// Entrando en comillas
				current_quote = c;
				quote_state = 1;
			}
			else if (c == current_quote)
			{
				// Saliendo de comillas
				quote_state = 0;
				current_quote = 0;
			}
			// Si es comilla diferente dentro de comillas, se trata como literal
		}
		// Solo terminar si encontramos pipe fuera de comillas
		else if (quote_state == 0 && is_pipe(c))
		{
			break;
		}        
		index++;
	}

	// Extraer el chunk de comando (sin incluir el pipe)
	chunk_input = ft_substr(shell->input, index_first_char, (index - index_first_char));
	if (!chunk_input)
		return (FAILURE);
	
	// Eliminar espacios al final del chunk
	len_input = ft_strlen(chunk_input);
	while (len_input > 0 && is_space(chunk_input[len_input - 1]))
	{
		chunk_input[len_input - 1] = '\0';
		len_input--;
	}
	
	// Solo añadir si no está vacío
	if (len_input > 0)
	{
		printf("Generated command: '%s'\n", chunk_input);
		add_command_node(&shell->command_list, chunk_input);
	}
	
	free(chunk_input);
	return (index - index_first_char);
}

int is_pipe(char character)
{
	int	pipe;

	pipe = 0;
	if (character == '|')
		pipe = 1;
	return (pipe);
}