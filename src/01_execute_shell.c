/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_execute_shell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 19:42:52 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	recover_previous_status(t_shell *shell);
void	read_user_input(t_shell *shell, char *prompt);
void	process_input(t_shell *shell);
void	process_commands(t_shell *shell);

void	execute_shell(t_shell *shell)
{
	int		iteration;
	
	if (!shell)											
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	iteration = 0;										// DEBUG CONTADOR 
	while (1)											
	{
		iteration++;
        printf("\n\n=== Input() ITERATION %d ===\n\n", iteration);
		
		recover_previous_status(shell);		// JUANJE -> ft_setup_signals() dentro 
		read_user_input(shell, PROMPT);					
		process_input(shell);
		
		//print_config_shell(shell);		// DEBUG
		free_iteration_input(shell);
		printf("DEBUG: Memory freed, iteration %d\n", iteration);
	}	
	free_iteration_input(shell);
}

// GESTION SIGNALS + COPIA EXIT_STATUS de la iteracion anterior a Shell
void	recover_previous_status(t_shell *shell)
{
	(void)	shell;			// TEMPORAL
	// CASO EOF (Ctrl+D o fin de pipe)
	/* shell->last_exit_status = shell->exit_status;		//  guarda estado anterior
	
	//ft_setup_signals();					----> JUANJE 

	if (g_signal_flag)
		shell->last_exit_status = g_signal_flag;		// SEÑALES (Ctrl+C = 130)
	else
		shell->exit_status = SUCCESS;	 */				// RESET A 0 SI NO HAY SEÑALES
}	

void	read_user_input(t_shell *shell, char *prompt)
{
	char	*input;	

	input = readline(prompt);
	if (!input)
		return ;		
	if (input[0] == '\0')					// CASO INPUT VACÍO - CONTINUAR SIN PROCESAR
	{
		free(input);
		return ;
	}					
	add_history(input);						// añadir a historial si input no vacio	
	shell->input = ft_strdup(input);		// PROCESAR INPUT VÁLIDO
	if (!shell->input)
	{
		free(input);
		return (ft_putendl_fd(ERROR_INPUT_READER, STDERR_FILENO));
	}
	free(input);
}

void	process_input(t_shell *shell)
{
	if (!shell || !shell->input)
		return ;
		
	if (validate_syntax(shell) == SYNTAX_ERROR)		
		return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO));

	create_commands_structure(shell);	
	if (validate_command_structure(shell) == SYNTAX_ERROR)		
        return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO));

	lexical_analyzer(shell->commands_list);	
	process_commands(shell);

	if (validate_command_semantics(shell) == SYNTAX_ERROR)		
		return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO));
		
	build_execution_structure(shell->commands_list);		

	// EJECUTAR COMANDOS 							!!! JUANJE
    // execute_commands(shell->command_list);
	
	print_commands_list(shell->commands_list);			// Debug
	free_commands_list(&shell->commands_list);
}

void	process_commands(t_shell *shell)
{
	t_cmd	*current_command;

	if (!shell)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	current_command = (t_cmd *) shell->commands_list;
	while (current_command)
	{
		tokenizer(current_command->words_list, shell);		
		variable_expander(current_command->words_list, shell);		
		dequotize_tokens(current_command->words_list, shell);
		generate_processed_word(current_command->words_list, shell);			
		current_command = current_command->next;
	}	
}
