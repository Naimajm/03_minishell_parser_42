/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:32:54 by juagomez          #+#    #+#             */
/*   Updated: 2025/04/30 21:25:33 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// FUNCIONES GITBOOK -> https://42-cursus.gitbook.io/guide/3-rank-03/minishell/functions




// MAIN -----------------------------------------------------------------------

int	main(void)
{ 
	// recepcion comandos -> readline
	char	*rl;
	rl = readline("minishell$ ");
	printf("r1 -> %s\n", rl);

	char	*pwd;
	// devuelve ruta del directorio trabajo actual del proceso que realiza la llamada
	pwd = getcwd(NULL, 0);
	printf("pwd -> %s\n", pwd);

	// change directorio
	chdir("/home/naima/PROGRAMACION/CURSUS_42/02_SO_LONG_42");
	pwd = getcwd(NULL, 0);
	printf("pwd -> %s\n", pwd);


	return (SUCCESS);
}