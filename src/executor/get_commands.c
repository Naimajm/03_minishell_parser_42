/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumarque <jumarque@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:52:30 by sbenitez          #+#    #+#             */
/*   Updated: 2025/07/31 13:15:01 by jumarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_cmdsize(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int	ft_has_commands(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell || !shell->commands_list)
		return (0);
	cmd = shell->commands_list;
	while (cmd)
	{
		if (cmd->args && cmd->args[0])
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

