/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_exec_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:31:52 by emcorona          #+#    #+#             */
/*   Updated: 2025/07/31 10:38:17 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas
// subject: with no options
//para eliminar variables de entorno o de shell, y funciones del shell (con -f). ej. unset NOMBRE_VARIABLE
/* Comportamiento importante:
No muestra ningún output al ejecutarse con éxito
Retorna 0 si tiene éxito
No da error si intentas eliminar una variable que no existe
No puede eliminar variables de solo lectura 
codigos de salida de unset:
Código	Razón del Fallo
1	Identificador de variable inválido. manejarlo con ft_check_env_var
1	Intento de eliminar una variable readonly.  manejarlo con ft_readonly_env_var
2	Se proporcionó una opción inválida. (el subject no lo contempla)
0	Éxito (incluso si la variable no existía).
VIP : unset puede recibir múltiples variables para eliminar.
Un nombre de variable válido no puede contener el signo =
*/

int			exec_unset(t_shell *shell, t_cmd *cmd);
static char	**ft_change_env(char **env, char *str);
static int	ft_is_readonly(t_shell *shell, char *var_name);
static void check_error_unset(t_shell *shell, t_cmd *cmd);

int	exec_unset(t_shell *shell, t_cmd *cmd)
{
	
	if (!shell || !cmd || !shell->environment || !cmd->args)
		return (1);
	shell->exit_status = 0; // inicializamos
	check_error_unset(shell, cmd);
	if (shell->exit_status = GEN_ERROR)
		return (shell->exit_status);
	//else
		
/* 	static void check_error_unset(t_shell *shell, t_cmd *cmd)
	{
		int i;

		i = 1; //  El bucle while debería empezar a procesar desde el primer argumento de unset, no desde el propio comando. cmd->args[0] es "unset", por lo que deberías empezar en i = 1.
		while (cmd->args[i]) 
		{
			
			
			if (!ft_valid_env_var(cmd->args[i]) || ft_strchr(cmd->args[i], '=')) // para chequear nombre invalido de variable. y verificacion de que no haya signo =
			{	// VER NOTA SOBRE IMPRESION DE ERRORES Construir el mensaje de error completo en STDERR
				ft_putstr_fd("minishell: unset: `", STDERR_FILENO); // sin salto de linea
				ft_putstr_fd(cmd->args[i], STDERR_FILENO); // sin salto de linea
				ft_putendl_fd("': not a valid identifier", STDERR_FILENO); // con salto de linea
				shell->exit_status = GEN_ERROR;
			}
			else if (ft_is_readonly(shell, cmd->args[i])) // readonly
			{
				ft_putstr_fd("minishell: unset: `", STDERR_FILENO); 
				ft_putstr_fd(cmd->args[i], STDERR_FILENO);
				ft_putendl_fd("': readonly variable", STDERR_FILENO);
				shell->exit_status = GEN_ERROR;
			}
			else
			{
				shell->environment = ft_change_env(shell->environment, cmd->args[i]);
				if (!shell->environment)
					return (1); // error de memoria
			}
			i++;
		}
	} */
	return (shell->exit_status); // en funcion del valor de shell->exit_status, ya sea cero o uno // unset puede recibir múltiples variables para eliminar, por eso retornamos el error ahora y no durante el ciclo while
}

static char	**ft_change_env(char **env, char *str)
{
	int index_env;
	int i;
	int j;
	int size_env;
	char **new_env;

	i = 0;
	j = 0;
	index_env = ft_search_index_env(env, str);
	if (index_env == -1)
		return (env);
	size_env = ft_matrix_size(env);
	new_env = (char **)ft_calloc(size_env, sizeof(char*)); // Usar ft_calloc en lugar de malloc para inicializar todo a NULL automáticamente
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (i != index_env) // no eliminamos una variable de entorno, si no que solo copiamos las que no coinciden con la que queremos elimar, es decir generamos un nuevo enviroment sin la que estamo queriendo quitar.  Es una forma segura y común de "eliminar" un elemento de un array de cadenas.
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j]) // comprobar quue lo anterior fue exitoso y si no liberar todo
			{
				while (j > 0)
					free(new_env[--j]);
				free(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	// new_env[j] = NULL; // tenemos sitio para cerrar el nuevo enviroment ya que al quitar una queda el sitio para el null sin tener que sumar uno  mas en el malloc.  // No es necesario añadir new_env[j] = NULL ya que ft_calloc ya inicializó todo a NULL
	free_matrix(env);
	return (new_env);
}

static int	ft_is_readonly(t_shell *shell, char *var_name)
{
	int	i;
	int	size_var;

	size_var = ft_strlen(var_name);
	i = 0;
	if (!var_name || !shell->readonly_vars)
	return (0);
	while (shell->readonly_vars[i])
	{
		if ((size_var == ft_strlen(shell->readonly_vars[i]))
			&& ft_strncmp(var_name, shell->readonly_vars[i], size_var) == 0)
			return (1); // la variable es readonly
		i++;
	}
	return (0); // la variable no es readonly
}

static void check_error_unset(t_shell *shell, t_cmd *cmd)
{
	int i;

	i = 1; //  El bucle while debería empezar a procesar desde el primer argumento de unset, no desde el propio comando. cmd->args[0] es "unset", por lo que deberías empezar en i = 1.
	while (cmd->args[i]) 
	{
		if (!ft_valid_env_var(cmd->args[i]) || ft_strchr(cmd->args[i], '=')) // para chequear nombre invalido de variable. y verificacion de que no haya signo =
		{	// VER NOTA SOBRE IMPRESION DE ERRORES Construir el mensaje de error completo en STDERR
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO); // sin salto de linea
			ft_putstr_fd(cmd->args[i], STDERR_FILENO); // sin salto de linea
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO); // con salto de linea
			shell->exit_status = GEN_ERROR;
		}
		else if (ft_is_readonly(shell, cmd->args[i])) // readonly
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO); 
			ft_putstr_fd(cmd->args[i], STDERR_FILENO);
			ft_putendl_fd("': readonly variable", STDERR_FILENO);
			shell->exit_status = GEN_ERROR;
		}
		else
		{
			shell->environment = ft_change_env(shell->environment, cmd->args[i]);
			if (!shell->environment)
				return (1); // error de memoria
		}
		i++;
	}
}

// NOTAS:

/* ft_putstr_fd("minishell: unset: ", STDERR_FILENO); 
printf("%s: invalid identifier\n", cmd->args[i]);
Estás mezclando ft_putstr_fd (que escribe en STDERR) con printf (que escribe en STDOUT).
El resultado es que una parte del mensaje de error (minishell: unset: ) va a la salida de error, pero el resto (nombre-invalido: invalid identifier) va a la salida estándar. Un shell debe enviar todo el mensaje de error a STDERR. */