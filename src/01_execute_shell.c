/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_execute_shell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/02 11:37:05 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//void	recover_previous_status(t_shell *shell);
int	read_user_input(t_shell *shell, char *prompt);
//void	read_user_input(t_shell *shell, char *prompt);
void	process_input(t_shell *shell);
void	process_commands(t_shell *shell);

void	execute_shell(t_shell *shell)
{
	//int		iteration;
	
	if (!shell)											
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	//iteration = 0;										// DEBUG CONTADOR 
	while (1)											
	{
		//iteration++;
        //printf("\n\n=== Input() ITERATION %d ===\n\n", iteration);
		
		//recover_previous_status(shell);		// JUANJE -> ft_setup_signals() dentro 

		/* printf("ANTES read_user_input -> last_exit_status -> %i\n", shell->last_exit_status);
		printf("exit_status -> %i\n", shell->exit_status); */
		
		if (read_user_input(shell, PROMPT) == FAILURE)
			break ;
		/* printf("ANTES process_input -> last_exit_status -> %i\n", shell->last_exit_status);
		printf("exit_status -> %i\n", shell->exit_status); */

		process_input(shell);
		
		//print_config_shell(shell);		// DEBUG
		free_iteration_input(shell);
		//printf("DEBUG: Memory freed, iteration %d\n", iteration);

		/* printf("DESPUES  process_input-> last_exit_status -> %i\n", shell->last_exit_status);
		printf("exit_status -> %i\n", shell->exit_status); */
	}	
	free_iteration_input(shell);
}

// GESTION SIGNALS + COPIA EXIT_STATUS de la iteracion anterior a Shell
/* // Modificado por Juan Jesus <-----------------------------------------------------
void	recover_previous_status(t_shell *shell)
{
	//(void)	shell;			// TEMPORAL
	// CASO EOF (Ctrl+D o fin de pipe)
	shell->last_exit_status = shell->exit_status;		//  guarda estado anterior	
	ft_setup_signals();
	if (g_signal_flag)
		shell->last_exit_status = g_signal_flag;		// SEÑALES (Ctrl+C = 130)
	else
		shell->exit_status = SUCCESS;	 				// RESET A 0 SI NO HAY SEÑALES
}	 */

int	read_user_input(t_shell *shell, char *prompt)
{
	char	*input;	

	shell->last_exit_status = shell->exit_status;		// 1. GUARDAR estado anterior ANTES de signals	
	ft_setup_signals();

	input = readline(prompt);
	if (!input)
		return (ft_putendl_fd("exit\n", STDOUT_FILENO), FAILURE);

	if (g_signal_flag)
	{
		shell->exit_status 			= g_signal_flag;
		shell->last_exit_status 	= g_signal_flag; // SEÑALES (Ctrl+C = 130)
		g_signal_flag 				= 0; 					// Reset después de procesar
		
	}				
	/* else
		shell->exit_status = SUCCESS; */	
		
	if (input[0] == '\0')					// CASO INPUT VACÍO - CONTINUAR SIN PROCESAR
	{
		free(input);
		return (SUCCESS);
	}					
	add_history(input);						// añadir a historial si input no vacio	
	shell->input = ft_strdup(input);		// PROCESAR INPUT VÁLIDO
	if (!shell->input)
	{
		free(input);
		shell->exit_status = ERROR;
		return (ft_putendl_fd(ERROR_INPUT_READER, STDERR_FILENO), FAILURE);
	}
	free(input);
	return (SUCCESS);
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

	process_commands(shell);

	if (validate_command_semantics(shell) == SYNTAX_ERROR)		
		return (ft_putendl_fd(ERROR_CHECK_SYNTAX, STDERR_FILENO));
		
	build_execution_structure(shell->commands_list);		

	// EJECUTAR COMANDOS 	-------------------------->	!!! JUANJE

	if (shell->commands_list)
		ft_exec_commands(shell);
	
    // execute_commands(shell->command_list); --------> !!! JUANJE
	
	//print_commands_list(shell->commands_list);			// Debug
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
		lexical_analyzer(current_command, shell);
		tokenizer(current_command->words_list, shell);		
		variable_expander(current_command->words_list, shell);		
		dequotize_tokens(current_command->words_list, shell);
		generate_processed_word(current_command->words_list, shell);			
		current_command = current_command->next;
	}	
}
