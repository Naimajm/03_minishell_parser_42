/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:35:28 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/07 18:33:12 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"


t_shell *initialize_shell(void)
{
	t_shell	*shell;

	/* if (!input)
		return (NULL);	 */
	shell = (t_shell *) malloc(sizeof(t_shell));
	if(!shell)
		return (NULL);	
		
	shell->input 		= NULL;
	shell->token_list 	= NULL;

	return (shell);
}