/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:52:54 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/05 20:22:41 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../inc/minishell.h"

// TODO, ESTRUCTURAR CÓDIGO, GENERAR STATIC Y FUNCION PRINCIPAL AL PRINCIPIO Y DESPUES LAS ESTATICAS
// TODO, MIRAR MI PIPES PARA LAS REDIRECCIONES.
static int	redir_heredoc(t_shell *shell, t_cmd *cmd);
static int	redir_infile(char *infile);
static int	redir_outfile(char *outfile, int append);

int	redirections(t_shell *shell, t_cmd *cmd)
{
	if (cmd->hd && redir_heredoc(shell, cmd))
		return (1);
	if (!cmd->hd && cmd->infile && redir_infile(cmd->infile))
		return (1);
	if (cmd->outfile && redir_outfile(cmd->outfile, cmd->append))
		return (1);
	return (0);
}

static int	redir_heredoc(t_shell *shell, t_cmd *cmd)
{
	int		pipefd[2];
	char	*buffer;

	g_signal_flag = 2; // ***Importante: Establece la bandera para Ctrl+C en here-document***
	if (pipe(pipefd) == -1)
		return (perror("Error pipe\n"), 1);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || !ft_strcmp(buffer, cmd->delimiter))
		{
			free(buffer);
			break ;
		}
		buffer = expand_heredoc(buffer, shell->environment, shell->last_exit_status);
		write(pipefd[1], buffer, ft_strlen(buffer));
		write(pipefd[1], "\n", 1);
		free(buffer);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (perror("Error heredoc\n"), 1);
	return (close(pipefd[0]), 0);
}

static int	redir_infile(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error duplicating input file\n");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redir_outfile(char *outfile, int append)
{
	int	fd;

	if (append == 1)
		fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error duplicating output file\n");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}


