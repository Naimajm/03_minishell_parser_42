/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_shell_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/21 13:36:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validate_environment(char **environment)
{
	if (!environment | !*environment)
		return (0);
	return (1);
}

t_shell *initialize_shell(void)
{
	t_shell	*shell;

	shell = (t_shell *) malloc(sizeof(t_shell));
	if (!shell)
		perror_exit(ERROR_MEMORY_ALLOCATION, STDERR_FILENO, FAILURE);
		
	shell->input 			= NULL;
	shell->environment		= NULL;

	shell->exit_status		= SUCCESS;
	shell->last_exit_status = SUCCESS;

	shell->commands_list	= NULL;		
	return (shell);
}

int	load_environment_variables(t_shell *shell, char **environment)
{
	int	env_count;
	int	index;

	if (!shell || !environment)
		perror_exit(ERROR_ENVIRONMENT, STDERR_FILENO, FAILURE);
	env_count 	= 0;
	index 		= 0;	
	while (environment[env_count]) 		// calculo total elementos
		env_count++;
	
	// reserva + carga copia en shell
	shell->environment = (char **) malloc(sizeof(char *) * (env_count + 1));
	// validacion estado carga variables
	if (env_count <= 0 || !shell->environment)
		perror_exit(ERROR_ENVIRONMENT, STDERR_FILENO, FAILURE);

	while (environment[index])
	{
		shell->environment[index] = ft_strdup(environment[index]);
		if (!shell->environment[index])
			perror_exit(ERROR_ENVIRONMENT, STDERR_FILENO, FAILURE);
		index++;
	}
	shell->environment[index] = NULL;	// terminador nulo char **
	return (SUCCESS);
}
