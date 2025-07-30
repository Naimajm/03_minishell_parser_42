/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:32:54 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/30 13:12:38 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	run_shell(t_shell *shell);
void	process_commands(t_shell *shell);
void	process_input(t_shell *shell);
char	*read_user_input(char *prompt);

// MAIN -----------------------------------------------------------------------

int	main(int	argc, char **argv, char **environment)
{
	t_shell	*shell;

	(void) argc;
	(void) argv; 
	if (validate_environment(environment))	
		return (ft_putendl_fd(ERROR_ENVIRONMENT, STDERR_FILENO), FAILURE);

	//printf("Shell initialization...\t\t\t OK\n\n");
	shell = initialize_shell();	
	
	//printf("Loading environment variables...\t\t OK\n\n");
	if (load_environment_variables(shell, environment) == FAILURE)		
	{
        cleanup_minishell(shell);
		ft_putendl_fd(ERROR_ENVIRONMENT, STDERR_FILENO);
        return (FAILURE);
    }

	// test parser
	if (argc > 1 && ft_strncmp(argv[1], "--test_basic", 12) == 0)
		test_basic_parser(shell);
	if (argc > 1 && ft_strncmp(argv[1], "--test", 6) == 0)
		test_complex_parser(shell);
	
	//printf("Run Shell...\t\t\t\t OK\n\n");
	run_shell(shell);	

    cleanup_minishell(shell);		
	return (SUCCESS);
}

void	run_shell(t_shell *shell)
{
	char	*input;
	int		iteration;
	
	if (!shell)											
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	iteration = 0;										// CONTADOR DEBUG
	while (1)											// loop ppal
	{
		iteration++;
        printf("\n\n=== Input() ITERATION %d ===\n\n", iteration);

		//printf("Input Reading...\t\t\t OK\n\n");
		input = read_user_input(PROMPT);
		if (!input)										// CASO EOF - TERMINAR SHELL
			break ;	
        if (input[0] == '\0')							// CASO INPUT VACÍO - CONTINUAR SIN PROCESAR
            continue;  									// VOLVER AL INICIO DEL LOOP
		shell->input = ft_strdup(input);				// PROCESAR INPUT VÁLIDO
		if (!shell->input)
			return (ft_putendl_fd(ERROR_MEMORY_ALLOC, STDERR_FILENO));	
			
		//printf("Input Processing... %s\n", shell->input);	
		process_input(shell);
		
		//print_config_shell(shell);		// DEBUG
						
		// LIBERAR INPUT
		free(input);
		free(shell->input);
		shell->input = NULL;

		printf("DEBUG: Memory freed, iteration %d\n", iteration);
	}	
}

void	process_input(t_shell *shell)
{
	if (!shell || !shell->input)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));	
		
	//printf("Check Syntax...\t\t\t OK\n");
	if (validate_syntax(shell) == SYNTAX_ERROR)		
	{
		ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO);
        return ;
    }

	//printf("Syntax analyzer...\t\t\t OK\n");
	create_commands_structure(shell);	

	//printf("Lexical analyzer...\t\t\t OK\n");
	lexical_analyzer(shell->commands_list);	
	//print_commands_list(shell->commands_list);		// Debug

	//printf("Process Comands...\t\t\t OK\n");
	process_commands(shell);

	//printf("Generating execution structure...\t OK\n");
	build_execution_structure(shell->commands_list);		

	// EJECUTAR COMANDOS !!! JUANJE
    // execute_commands(shell->command_list);
	
	print_commands_list(shell->commands_list);			// Debug

	// LIBERAR ESTRUCTURAS COMMANDS
	free_commands_list(&shell->commands_list);
	shell->commands_list = NULL;
}

void	process_commands(t_shell *shell)
{
	t_cmd	*current_command;

	if (!shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_command = (t_cmd *) shell->commands_list;
	while (current_command)
	{
		//printf("Word -> Tokenizer...				OK\n\n");
		tokenizer(current_command->words_list);

		//printf("Tokens -> Expand variables $...			OK\n\n");
		variable_expander(current_command->words_list, shell->environment, shell->exit_status);

		//printf("Tokens -> Dequotize tokens...			OK\n\n");
		dequotize_tokens(current_command->words_list);	

		//printf("Word -> Join Tokens to 'processed_word'...	OK\n\n");
		generate_processed_word(&current_command->words_list);	
		
		current_command = current_command->next;
	}	
}

char	*read_user_input(char *prompt)
{
	char	*input;

	input = readline(prompt);

	// CASO EOF (Ctrl+D o fin de pipe)

	if (!input)
	{
		return (NULL); 		// Retornar NULL para indicar EOF	
	}
	
	if (input[0] != '\0')  	// añadir a historial si input no vacio	
		add_history(input);
	return (input);
}


