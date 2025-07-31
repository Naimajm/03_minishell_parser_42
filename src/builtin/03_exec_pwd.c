/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_exec_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 09:52:44 by emcorona          #+#    #+#             */
/*   Updated: 2025/07/30 20:40:28 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

// TODO REDIRECCIONAR COMO EN ECHO ????

int	exec_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0); // asigna memoria dinámicamente al pasarn NULL como argumento y cero como tamaño de buffer, por lo que el tamaño será determinado por la función.
	if (!pwd)
	{
		perror("minishell: pwd:"); // perror para errores del sistema (open, read, dup,....)
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
