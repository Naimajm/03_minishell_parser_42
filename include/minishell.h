/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/04/30 20:11:04 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// EXTERNAL LIBRARIES

#include <unistd.h>	// write getcwd chdir
#include <stdio.h>  // printf aux

#include <readline/readline.h> 	// readline
#include <readline/history.h>	// readline


// OWN LIBRARIES

#include	"../libft/libft.h"
#include	"../ft_printf/ft_printf.h"

// MACROS

# define SUCCESS	0
# define FAILURE	1

# define ERROR_ARGS		"Error\n error number arguments\n"


#endif