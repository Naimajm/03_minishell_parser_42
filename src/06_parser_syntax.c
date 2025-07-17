/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_parser_syntax.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:44:56 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/17 10:31:52 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



// IGUAL FUNCIONALIDAD QUE ft_process_token ¡¡
/* void create_command_structure__cambiar(t_word_token *words_list, t_command **commands)
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
} */

