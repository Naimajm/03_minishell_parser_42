/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:32:54 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/05 18:37:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	*input_reader(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
		return (NULL);
	// añadir a historial si input no vacio	
	if (input[0] != '\0') 
		add_history(input);
	return (input);
}

void	start_minishell(char *prompt)
{
	char	*input;
	t_shell	*shell;
	
	

	input = NULL;
	// inicializar shell
	shell = initialize_shell();
	
	// loop ppal
	while (1)
	{
		// LEER INPUT
		input = input_reader(prompt);
		if (!input)
			break;
		shell->input = input;
		ft_printf("Procesando: %s\n", shell->input);

		// PARSEAR INPUT
		categorize_token(shell);
		
		// EJECUTAR COMAND

		// LIBERAR INPUT
		free(input);
		input = NULL;
	}	
	// free final
	if (shell)
		cleanup_minishell(shell);
}



// MAIN -----------------------------------------------------------------------

int	main(void)
{
	// DASHBOARD PROYECTO
	print_text_file("work_process.txt");	
	
	start_minishell(PROMPT);

	return (SUCCESS);
}