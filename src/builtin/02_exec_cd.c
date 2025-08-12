/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_exec_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:13:03 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/06 19:06:49 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

// TODO COMPARAR LOS TEST BIEN CON LA SALIDA DE BASH, YA QUE HE HECHO MUY SENCILLA ESTA FUNCIÓN
//int	exec_cd(t_cmd *cmd, t_shell *shell)
int	exec_cd(t_cmd *cmd) // según el subject, solo para path relativos o absoolutos, no hay que manejar el ir a $HOME en caso de no argumentos, ni actualizar las variables de entorno PWD y OLDPWD, ni Manejar el caso cd - (ir al directorio anterior)
{
	char	*path;

	if (cmd->args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO); // para errores de sintaxis o uso del comando ft_putstr_fd es mas apropiado. Sigue el formato estándar de shell minishell: comando: mensaje
		return (ERROR);
	}
	path = cmd->args[1];
	if (!path || path[0] =='\0') // verifica que el path no sea una cadena vacia
	{
		ft_putendl_fd("minishell: cd: a path is needed", STDERR_FILENO);
		return (ERROR);
	}
	if (chdir(path) == -1) // chdir funciona tanto con paths relativos como absolutos.  mostrando el mensaje de error proporcionado por perror(), que incluirá el motivo específico del fallo. Por ejemplo: minishell: cd: No such file or directory , minishell: cd: Not a directory, minishell: cd: Permission denied
	{
		perror("minishell: cd:"); // perror para errores del sistma (chdir)
		//shell->exit_status = ERROR; // estado de salida en caso de error del ultimo comando ejecutado
		return (ERROR);
	}
	return (SUCCESS);
}

/* El subject no pide lo siguiente
Manejar el caso sin argumentos (ir a $HOME)
Manejar el caso cd - (ir al directorio anterior)
Actualizar las variables de entorno PWD y OLDPWD */

/* La implementación correcta del comando cd en un shell mas complejo normalmente debería incluir:

Manejo de cd sin argumentos (ir a HOME)
Actualización de las variables de entorno PWD y OLDPWD
Manejo de cd - (ir al directorio anterior)
Manejo de paths relativos y absolutos */