/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_command_generate.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:01:32 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 14:09:50 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// 	ANALISIS SINTACTICO PARA GENERACION LISTA DE COMANDOS

void	create_commands_structure(t_shell *shell);
int 	generate_command(t_shell *shell, int start_index);
char	*create_clean_command(char *input, int start_index, int final_index);
int 	is_pipe(char character);


// AGRUPAR INPUT NO PROCESADOS PARA DIFERENTES JOBS EN EL CASO DE PIPES
void	create_commands_structure(t_shell *shell)
{
	int		index;
	int		input_len;

	if (!shell || !shell->input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));			
	index = 0;	

	while (shell->input[index])
	{
		while (is_space(shell->input[index])) 	// ignorar espacios iniciales 
			index++;
		if (!shell->input[index]) 				// Verificar si llegamos al final después de saltar espacios
			break;
		
		input_len = generate_command(shell, index);	// CLASIFICACION LISTA PROCESOS		
        if (input_len <= 0)						// Verificar que se generó un comando válido
        {            
            index++;							// Si no se pudo generar comando válido, avanzar un carácter
            continue;
        }
		index = advance_index_by_length(index, input_len);	// GESTION CASOS ESPECIALES AVANCE INDEX

		// CASO PIPE -> ' | ' (operador aislado con espacios)
		while (is_space(shell->input[index])) 	// ignorar espacios iniciales 
			index++;
		
		if (is_pipe(shell->input[index]))		// Si encontramos un pipe, saltarlo para el siguiente comando
		{
			printf("Found pipe, creating new command structure\n");
			index++; 							// Saltar el pipe			
		}	
	}	
}

int generate_command(t_shell *shell, int start_index)
{
	char	*command_input;
	int		index;
	int		command_len;

	bool	inside_quotes;
	char	active_quote_char;
	char 	character;

	if (!shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	index 				= start_index;
	inside_quotes 		= false;
	active_quote_char 	= 0;

	// Procesar hasta encontrar pipe fuera de comillas o final de input
	while (shell->input[index])
	{
		character = shell->input[index];
		
		// Manejar comillas
		if (is_quote(character))
		{
			if (inside_quotes == false) // Entrando en comillas
			{				
				active_quote_char = character;
				inside_quotes = true;
			}
			else if (character == active_quote_char) // Saliendo de comillas
			{	
				active_quote_char = 0;			
				inside_quotes = false;				
			}
			// Si es comilla diferente dentro de comillas, se trata como literal
		}
		// Solo terminar si encontramos pipe fuera de comillas
		else if (inside_quotes == false && is_pipe(character))
			break;    
		index++;
	}
	// Extraer el comando (sin incluir el pipe)
	command_input = create_clean_command(shell->input, start_index, index);
	if (!command_input)
		return (ft_putendl_fd(ERROR_INIT, STDERR_FILENO), FAILURE);	
	
	command_len = ft_strlen(command_input);
	// AÑADIR NODO COMMAND
	if (command_len > 0)
	{
		printf("Generated command: <%s>\n", command_input);		
		add_command_node(&shell->commands_list, command_input);
		printf("DEBUG: add command successfully -> command_len = %d\n",command_len);
	}	
	free(command_input);
	//printf("DEBUG: returning \n", command_len);
	return (command_len);
}

char	*create_clean_command(char *input, int start_index, int final_index)
{
	char	*raw_command;
	char	*clean_command;
	int		len;

	if (!input || start_index >= final_index)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);	

	// Extraer el comando (sin incluir el pipe)
	raw_command = ft_substr(input, start_index, (final_index - start_index));
	if (!raw_command)
		return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);

	// eliminar espacios al final
	len = ft_strlen(raw_command);
	while (len > 0 && is_space(raw_command[len - 1]))
	{
		raw_command[len - 1] = '\0';
		len--;
	}
	// Crear copia limpia
	clean_command = ft_strtrim(raw_command, " \t\n\r");
	if (!clean_command)
	{
    	free(raw_command);
    	return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO), NULL);
	}
	free(raw_command);
	return (clean_command);
}

