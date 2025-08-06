/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02.00_executer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:52:36 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/05 20:21:39 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static pid_t	exec_single_cmd(t_cmd *cmd, int *prevfd, int *pipefd, t_shell *ms);
static void		child_process(t_cmd *cmd, int prevfd, int pipefd[2], t_shell *ms);
static void		parent_process(t_shell *ms, int *prevfd, int pipefd[2]);
static void		wait_all_processes(pid_t *pids, t_shell *ms);

void	exec_commands(t_shell *ms)
{
	int		pipefd[2];
	int		prevfd;
	t_cmd	*cmd;
	pid_t	pids[LOL];
	int		i;

	i = 0;
	cmd = ms->commands_list;
	prevfd = -1;
	if (cmd->is_btn && !cmd->next && (prevfd == -1))
	{
		exec_builtins(ms, cmd, prevfd);
		return ;
	}
	while (cmd)
	{
		pids[i] = exec_single_cmd(cmd, &prevfd, pipefd, ms);
		cmd = cmd->next;
		i++;
	}
	wait_all_processes(pids, ms);
}

static pid_t	exec_single_cmd(t_cmd *cmd, int *prevfd, int *pipefd, t_shell *ms)
{
	pid_t	pid;

	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("Error creating pipe\n");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error creating child process.\n");
		exit(1);
	}
	if (pid == 0)
		child_process(cmd, *prevfd, pipefd, ms);
	else
	{
		parent_process(ms, prevfd, pipefd);
		if (cmd->is_btn && !ft_strncmp(cmd->args[0], "exit", 5)
			&& (*prevfd == -1))
			exit(ms->exit_status);
	}
	return (pid);
}

static void	child_process(t_cmd *cmd, int prevfd, int pipefd[2], t_shell *ms)
{
	if (redirections(ms, cmd) == 1)
		exit (1);
	if (prevfd != -1 && !cmd->infile) // TODO INCLUIDO DE JUAN: los errores de redireccionamiento de priorizar entre | y < ya está,  cuando hay un nombre en infile no modificas o duplicas el STDINPUT y asi prioriza en el redireccionamiento.
		dup2(prevfd, STDIN_FILENO);
	if (cmd->next && !cmd->outfile)
		dup2(pipefd[1], STDOUT_FILENO);
	if (prevfd != -1)
		close(prevfd);
	if (cmd->next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (cmd->is_btn)
	{
		exec_builtins(ms, cmd, prevfd);
		exit(ms->exit_status);
	}
	else
		execute_command(ms, cmd);
	perror("Error executing\n");
	exit (126);
}

static void	parent_process(t_shell *ms, int *prevfd, int pipefd[2])
{
	if (*prevfd != -1)
		close(*prevfd);
	if (ms->commands_list->next)
	{
		close(pipefd[1]);
		*prevfd = pipefd[0];
	}
	else
		*prevfd = -1;
	g_signal_flag = 1;
}

static void	wait_all_processes(pid_t *pids, t_shell *shell)
{
	int		status;
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = shell->commands_list;
	while (cmd)
	{
		waitpid(pids[i], &status, 0);
		i++;
		if (!cmd->next) //chequeamos el exitstatus
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status) % 256;
			else if (WIFSIGNALED(status))
				shell->exit_status = (128 + WTERMSIG(status)) % 256;
		}
		cmd = cmd->next;
	}
}



