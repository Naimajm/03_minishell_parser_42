/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:32:54 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/15 00:43:25 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	input_parser(t_shell *shell);
char	*input_reader(char *prompt);

void	start_minishell(char *prompt, char **environment_var)
{
	char	*input;
	t_shell	*shell;	
	
	if (!prompt || !environment_var)						// validacion inputs
		return ;
	input = NULL;
	
	shell = initialize_shell();								// inicializar shell	
	load_environment_variables(shell, environment_var);		// cargar variables entorno
	
	// loop ppal
	while (1)
	{
		// LEER INPUT
		input = input_reader(prompt);
		if (!input)
			break ;
		shell->input = input;
		ft_printf("Processing: %s\n", shell->input);

		// PARSEAR INPUT
		input_parser(shell);

		// validacion estructura shell
		//print_config_shell(shell);	
		
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
	printf("Word Tokenizer...				OK\n\n");
	lexical_analyzer(shell);
	//test_lexical_analyzer(shell);

	printf("Tokenizer...					OK\n\n");
	tokenizer(shell->words_list);

	// check error sintaxis tokens

	printf("Tokens -> Expand variables $...			OK\n\n");
	activate_expand_operators(shell->words_list, shell->environment, shell->exit_status);

	printf("Tokens -> dequotize and join $...		OK\n\n");
	dequotize(shell->words_list);	


	// verificacion	
	print_words_list(shell->words_list);	
}

char	*input_reader(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
		print_message_and_exit(ERROR_INPUT_READER, STDERR_FILENO, FAILURE);
	
	if (input[0] != '\0')  // añadir a historial si input no vacio	
		add_history(input);
	return (input);
}

// MAIN -----------------------------------------------------------------------

int	main(int	argc, char **argv, char **env)
{
	(void) argc;
	(void) argv;

	// DASHBOARD PROYECTO
	//print_text_file("_work_process.txt");	

	if (!*env)
		print_message_and_exit(ERROR_ENVIRONMENT, STDERR_FILENO, FAILURE);

	// INICIO
	start_minishell(PROMPT, env);
		
	return (SUCCESS);
}