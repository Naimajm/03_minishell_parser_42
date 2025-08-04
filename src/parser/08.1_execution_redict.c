/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08.1_execution_redict.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:16:26 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/04 21:55:29 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_infile(t_cmd *command, t_word *word);
void	process_outfile(t_cmd *command, t_word *word);
void	process_append(t_cmd *command, t_word *word);
void	process_heredoc(t_cmd *command, t_word *word);

void	process_infile(t_cmd *command, t_word *word)
{
	//t_cmd	*current_command;

	//current_command = (t_cmd *) command;

	if (command->infile)
		free(command->infile);  ///  liberar existente si redict consedcutivos
	command->infile = ft_strdup(word->next->processed_word);
	if (!command->infile)
		return ;
			
	if (access(command->infile, R_OK) == FAILURE)		// verificacion permisos archivos
	{
		command->exit_status = ERROR;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command->infile, STDERR_FILENO);
		ft_putendl_fd(ERROR_FILE_NOT_FOUND, STDERR_FILENO);
	}
}

void	process_outfile(t_cmd *command, t_word *word)
{
	int	fd;

	if (command->outfile)
		free(command->outfile);
	command->outfile = ft_strdup(word->next->processed_word);
	if (!command->outfile)
		return ;
	command->append = false;
	
	fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == FAILURE)
	{
		command->exit_status = ERROR;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command->outfile, STDERR_FILENO);
		ft_putendl_fd(ERROR_PERMISSION_DENIED, STDERR_FILENO);
	}
	else
	{
		if (close(fd) == FAILURE)
		{
			command->exit_status = ERROR;
			ft_putendl_fd(ERROR_CLOSING_FILE, STDERR_FILENO);
		}
	}
}

void	process_append(t_cmd *command, t_word *word)
{
	int	fd;

	if (command->outfile)
		free(command->outfile);
	command->outfile = ft_strdup(word->next->processed_word);
	if (!command->outfile)
		return ;

	command->append = true;
	fd = open(command->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == FAILURE)
	{
		command->exit_status = ERROR;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command->outfile, STDERR_FILENO);
		ft_putendl_fd(ERROR_PERMISSION_DENIED, STDERR_FILENO);
	}
	else
	{
		if (close(fd) == FAILURE)
		{
			command->exit_status = ERROR;
			ft_putendl_fd(ERROR_CLOSING_FILE, STDERR_FILENO);
		}
	}
}

void	process_heredoc(t_cmd *command, t_word *word)
{
	if (command->delimiter)
		free(command->delimiter);
	command->delimiter = ft_strdup(word->next->processed_word);
	if (!command->delimiter)
		return ;
		
	command->hd = true;
}