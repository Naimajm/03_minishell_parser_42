/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:28:00 by sbenitez          #+#    #+#             */
/*   Updated: 2025/07/31 12:17:42 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" //  ../.. segun los niveles de carpetas

// TODO, ESTAS FUNCIONES YA ESTAN EN LA PARTE DE PARSEO , AUNQUE CON MODIFICACION DE LOS NOMBRES. HABRIA QUE ADAPTAR LAS LLAMADAS A ELLAS EN LA PARTE DE EXECUTOR

/* void	ft_free_matrix(char **matrix)
{
	int	i;

	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		if (matrix[i])
			free(matrix[i]);
		i++;
	}
	free(matrix);
	matrix = NULL;
} */

/* void	ft_free_tknlst(t_token **token)
{
	t_token	*temp;

	while (*token)
	{
		temp = (*token)->next;
		if ((*token)->raw_token)
			free((*token)->raw_token);
		free(*token);
		*token = temp;
	}
	*token = NULL;
} */

/* void	ft_free_cmdlst(t_cmd **cmd_lst)
{
	t_cmd	*temp;
	t_cmd	*next;

	temp = *cmd_lst;
	while (temp)
	{
		next = temp->next;
		ft_free_cmd_args(temp);
		ft_free_cmd_files(temp);
		free(temp);
		temp = next;
	}
	*cmd_lst = NULL;
} */

/* void	ft_cleanup_shell(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
	{
		ft_free_tknlst(&shell->raw_token);
		shell->token = NULL;
	}
	if (shell->cmd_lst)
	{
		ft_free_cmdlst(&shell->cmd_lst);
		shell->cmd_lst = NULL;
	}
} */

/* void	ft_clean(char **matrix, t_token *token, t_shell *shell)
{
	if (matrix)
		ft_free_matrix(matrix);
	if (token)
		ft_free_tknlst(&token);
	if (shell)
	{
		free(shell);
		shell = NULL;
	}
} */
