/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:32:54 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/22 00:19:39 by juagomez         ###   ########.fr       */
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
	if (!validate_environment(environment))		
		perror_exit(ERROR_ENVIRONMENT, STDERR_FILENO, GENERAL_ERROR);

	printf("Shell initialization...\t\t\t OK\n\n");
	shell = initialize_shell();	
	
	printf("Loading environment variables...\t\t\t OK\n\n");
	if (load_environment_variables(shell, environment) == GENERAL_ERROR)		
	{
        cleanup_minishell(shell);
        perror_exit(ERROR_ENVIRONMENT, STDERR_FILENO, GENERAL_ERROR);
    }
	
	printf("Run Shell...\t\t\t\t OK\n\n");
	run_shell(shell);

    cleanup_minishell(shell);		
	return (SUCCESS);
}

void	run_shell(t_shell *shell)
{
	char	*input;
	
	if (!shell)											
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
	while (1)											// loop ppal
	{
		printf("Input Reading...\t\t\t OK\n\n");
		input = read_user_input(PROMPT);
		if (!input)
			break ;
		shell->input = ft_strdup(input);				// Copia segura input
		if (!shell->input)
		{
			free(input);
			perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, GENERAL_ERROR);
		}		
			
		printf("Input Processing... %s\n", shell->input);	
		process_input(shell);
		
		//print_config_shell(shell);					
		// LIBERAR INPUT
		free(input);
		free(shell->input);
		shell->input = NULL;
	}	
}

void	process_input(t_shell *shell)
{
	char *syntax_error;

	if (!shell || !shell->input)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);

	/* printf("Syntax Check...\t\t\t OK\n");
	if (!validate_syntax(shell))		
		perror_exit(validate_syntax(shell), STDERR_FILENO, SYNTAX_ERROR); */		

	printf("Syntax analyzer...\t\t\t OK\n");
	syntax_analyzer(shell);

	/* printf("Syntax validation...\t\t\t ");
    syntax_error = validate_command_structure(shell->commands_list);
    if (syntax_error)
    {
        printf("ERROR\n");
        ft_putendl_fd(syntax_error, STDERR_FILENO);
        shell->exit_status = SYNTAX_ERROR;
        return ;
    }
    printf("OK\n"); */

	printf("Lexical analyzer...\t\t\t OK\n");
	lexical_analyzer(shell->commands_list);	

	printf("Process Comands...\t\t\t OK\n");
	process_commands(shell);

	printf("Generating execution structure...\t OK\n");
	build_execution_structure(shell->commands_list);	
	
	//printf("Test ->...					OK\n\n");
	//test_lexical_analyzer(shell);	

	// EJECUTAR COMANDOS
    // execute_commands(shell->command_list);
	
	print_commands_list(shell->commands_list);		// Debug

	// LIBERAR ESTRUCTURAS COMMANDS
	free_commands_list(&shell->commands_list);
	shell->commands_list = NULL;
}

void	process_commands(t_shell *shell)
{
	t_cmd	*current_command;

	if (!shell)
		perror_exit(ERROR_INVALID_INPUT, STDERR_FILENO, GENERAL_ERROR);
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
	if (!input)
		perror_exit(ERROR_INPUT_READER, STDERR_FILENO, GENERAL_ERROR);
	
	if (input[0] != '\0')  // añadir a historial si input no vacio	
		add_history(input);
	return (input);
}


