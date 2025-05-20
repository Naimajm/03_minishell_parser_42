/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/20 08:59:42 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

t_shell *initialize_shell(void);

// FUNCION CONSEGUIR EXIT_STATUS EN NUMERO PARA CASO EXPAND '$?'


// FUNCION GET Y SET PID SHELL 

void	load_environment_variables(t_shell *shell, char **environment_var)
{
	int	count;
	int	index;

	if (!shell || !environment_var)
		return ;
	count 	= 0;
	index 	= 0;
	// calculo total elementos
	while (environment_var[count])
		count++;
	
	// reserva + carga copia en shell
	shell->environment = (char **) malloc(sizeof(char *) * (count + 1));
	if (!shell->environment)
		free_matrix(shell->environment);

	while (environment_var[index])
	{
		shell->environment[index] = ft_strdup(environment_var[index]);
		index++;
	}
	shell->environment[index] = NULL;	// terminador nulo char **
}

t_shell *initialize_shell(void)
{
	t_shell	*shell;

	shell = (t_shell *) malloc(sizeof(t_shell));
	if(!shell)
		return (NULL);	
		
	shell->input 		= NULL;
	shell->environment	= NULL;
	shell->token_list 	= NULL;

	shell->exit_status	= 0;
	shell->last_exit_status = 0;
	return (shell);
}