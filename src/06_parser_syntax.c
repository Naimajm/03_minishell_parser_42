/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_syntax.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/17 00:38:56 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// 	ANALISIS SINTACTICO PARA GENERACION LISTA DE COMANDOS

void	syntax_analyzer(t_word_token *words_list, t_command **commands_list)
{
	// IDENTIFICAR OPERADORES
	

	// CREAR ESTRUCTURA COMANDOS
	// create_command_structure()
	
	// VALIDACION SINTACTICA
	//validate_syntax()
	
}


// IGUAL FUNCIONALIDAD QUE ft_process_token ¡¡
void create_command_structure(t_word_token *words_list, t_command **commands)
{
    t_command 		*current_cmd = NULL;
    t_word_token 	*current_word = words_list;
    
    while (current_word)
    {
        if (current_word->word_type == WORD)
        {
            // Añadir a argv del comando actual
            add_argument_to_command(current_cmd, current_word->processed_word);
        }
        else if (current_word->word_type != WORD)
        {
            handle_operator(current_word, &current_cmd, commands);
        }
        current_word = current_word->next;
    }
}

int validate_syntax(t_command *commands)
{
    // Verificar:
    // - No pipes al inicio/final
    // - No operadores consecutivos
    // - Argumentos después de redirects
    // - Comillas balanceadas (ya hecho)
    return (SUCCESS);
}