/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.01_executer_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:52:44 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/05 20:19:48 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"

static void	handle_command_execution(t_shell *shell, t_cmd *cmd, char *path);
static char	*search_in_paths(char **paths, char *comm);
static char	*ft_path(char *path, char **comm);

void	execute_command(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*path;
	char	**env;

	i = 0;
	env = shell->environment;
	if (!cmd->args || !cmd->args[0])
	{
		ft_putstr_fd("Error: empty command\n", 2);
		exit(127);
	}
	if (cmd->args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: command '' not found\n", 2);
		exit(127);
	}
	while (env[i] && ft_strnstr(env[i], "PATH", 4) == 0)
		i++;
	if (!ft_strchr(cmd->args[0], '/') && !(env[i]))
	{
		ft_putstr_fd("No PATH found\n", 2);
		exit(127);
	}
	path = ft_path((env[i]), cmd->args);
	handle_command_execution(shell, cmd, path);
}


static char	*ft_path(char *path, char **comm)
{
	char	**paths;

	if (ft_strchr(comm[0], '/'))
	{
		if (access(comm[0], F_OK | X_OK) == 0)
			return (ft_strdup(comm[0]));
		return (NULL);
	}
	if (!path)
		return (NULL);
	paths = ft_split(path + 5, ':');
	return (search_in_paths(paths, comm[0]));
}

static char	*search_in_paths(char **paths, char *comm)
{
	char	*route;
	char	*tmp;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		route = ft_strjoin(tmp, comm);
		free(tmp);
		if (access(route, F_OK | X_OK) == 0)
		{
			free_matrix(paths);
			return (route);
		}
		free(route);
		i++;
	}
	free_matrix(paths);
	return (NULL);
}



// TODO: AÑADIDO DE JUAN Y BAJADO A 25 LINEAS
static void	handle_command_execution(t_shell *shell, t_cmd *cmd, char *path)
{
	int	errno;
	
	if (!path)
	{
		if (access(cmd->args[0], X_OK) != 0 && access(cmd->args[0], F_OK) == 0
			&& ft_strchr(cmd->args[0], '/'))
		{
			ft_putstr_fd("invalid permission: ", STDERR_FILENO);
			ft_putendl_fd(cmd->args[0], STDERR_FILENO);
			errno = 126;
		}
		else
		{
			ft_putstr_fd("command not found: ", STDERR_FILENO);
			ft_putendl_fd(cmd->args[0], STDERR_FILENO);
			errno = 127;
		}
		free_matrix(cmd->args);
		exit(errno);
	}
	execve(path, cmd->args, shell->environment);
	free_matrix(cmd->args);
	free(path); // TODO: PUEDO QUITAR ESTE FREE ????
	ft_putendl_fd("Error executing.", STDERR_FILENO);
	exit(126);
}


