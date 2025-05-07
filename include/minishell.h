/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/07 14:40:38 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// EXTERNAL LIBRARIES
#include <unistd.h>	// write getcwd chdir
#include <stdio.h>  // printf aux
#include <fcntl.h>	// open

#include <readline/readline.h> 	// readline
#include <readline/history.h>	// readline


// OWN LIBRARIES
#include	"../libft/libft.h"
#include	"../ft_printf/ft_printf.h"

// MACROS
# define SUCCESS	0
# define FAILURE	1

# define ERROR_ARGS			"Error\n Error number arguments\n"
# define ERROR_OPEN_FILE	"Error\n Opening file\n"

# define PROMPT				"minishell$ "

# define FREE_ALL_SHELL		"Free\n Total cleaning minishell\n"
# define FREE_MATRIX		"Free\n cleaning matrix\n"

// categorizacion tokens
# define WORDS_NO_QUOTATION		0
# define WORDS_SINGLE_QUOTES	1
# define WORDS_DOUBLE_QUOTES	2
# define OUTFILE				3
# define APPEND					4
# define INFILE					5
# define HERE_DOC				6
# define PIPE					7

// TIPOS TOKEN
/* 0 -> palabras sin comillas
1 -> palabras con comillas simples -> literal
2 -> palabras con comillas dobles -> expansion variables
3 -> operador > OUTFILE
4 -> operador >> APPEND
5 -> operador < INFILE
6 -> operador <> HERE_DOC
7 -> operador | PIPE */

// STRUCT -----------------------------------------------------

// TOKEN
typedef struct s_token
{
	char	*token;
	int		type;
	//struct	s_token	*previous;	
	struct	s_token	*next;	
}			t_token;

// SHELL
typedef struct s_shell
{
	char	*input;
	t_token	*token_list;
}			t_shell;

// FUNCTIONS -----------------------------------------------------

// 00_minishell.c

char	*input_reader(char *prompt);
void	start_minishell(char *prompt);

// 01_init_shell.c

t_shell *initialize_shell(void);

// 02_parser_tokenize.c

void	categorize_token(t_shell *shell);

// 03_parser_struct_token.c

void	listtoken_add_back(t_token **token_list, char  *input, int token_type);
void print_token_list(t_token *token_list);
	
// 08_utils.c 

void	print_text_file(const char *filename);

// 09_free_functions.c

void	cleanup_minishell(t_shell *shell);
void	free_matrix(void **matrix);

#endif