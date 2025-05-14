/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:32:54 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/14 13:18:08 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	input_parser(t_shell *shell);
char	*input_reader(char *prompt);

void	start_minishell(char *prompt, char **environment_var)
{
	char	*input;
	t_shell	*shell;	

	// validacion inputs
	if (!prompt || !environment_var)
		return ;

	input = NULL;
	// inicializar shell
	shell = initialize_shell();

	// cargar variables entorno
	load_environment_variables(shell, environment_var);	
	
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
		input_parser(shell);

		// validacion estructura shell
		print_config_shell(shell);	
		
		// EJECUTAR COMAND

		// LIBERAR INPUT
		free(input);
		input = NULL;
	}	
	// free final
	if (shell)
		cleanup_minishell(shell);
}

void	input_parser(t_shell *shell)
{
	tokenizer(shell);

	// verificacion lista tokens	
	print_token_list(shell->token_list);

	// check error sintaxis tokens

	// expandir variables  "$VAR"
	if (search_expand_operators(shell->token_list))
	{
		ft_printf("expand var OK\n");
		activate_expand_operators(shell->token_list);
	}	
	
}

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

// MAIN -----------------------------------------------------------------------

int	main(int	argc, char **argv, char **env)
{
	(void) argc;
	(void) argv;

	// DASHBOARD PROYECTO
	print_text_file("_work_process.txt");	

	// INICIO
	start_minishell(PROMPT, env);

	return (SUCCESS);
}