/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_expand_exitstatus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:20:15 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/12 19:24:20 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// EXPANSION VARIABLES -> GESTION CASO ESPECIAL "$?"
// Devuelve el código de salida del último comando ejecutado

// GESTIONAR CASOS ESPECIALES $$ -> Devuelve el PID (Process ID) del shell actual.
// "$!" -> Devuelve el PID del último proceso ejecutado en segundo plano.
//???? GABI

