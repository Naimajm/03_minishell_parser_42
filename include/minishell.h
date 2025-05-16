/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/16 09:42:38 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// EXTERNAL LIBRARIES
#include <unistd.h>		// write getcwd chdir
#include <stdio.h>  	// printf aux
#include <fcntl.h>		// open
#include <stdbool.h>	// bool

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

# define ERROR_QUOTE_SYNTAX	"Error\n Simple/double quote syntax. Not closed\n"

# define PROMPT				"minishell$ "

# define FREE_ALL_SHELL		"Free\n Total cleaning minishell\n"
# define FREE_TOKENS		"Free\n cleaning Token List\n"
# define FREE_MATRIX		"Free\n cleaning matrix\n"

// categorizacion tokens
# define WORDS_NO_QUOTATION		0		// 0 -> palabras sin comillas
# define WORDS_SINGLE_QUOTES	1		// 1 -> palabras con comillas simples -> literal
# define WORDS_DOUBLE_QUOTES	2		// 2 -> palabras con comillas dobles -> expansion variables
# define OUTFILE				3		// 3 -> operador > OUTFILE
# define APPEND					4		// 4 -> operador >> APPEND
# define INFILE					5		// 5 -> operador < INFILE
# define HERE_DOC				6		// 6 -> operador <> HERE_DOC
# define PIPE					7		// 7 -> operador | PIPE

// STRUCT -----------------------------------------------------

// VARIABLES EXPANDIDAS
typedef struct s_expand
{
	int		first_index;
	int		last_index;
	char	*key;
	char	*value;		
}			t_expand;

// TOKEN
typedef struct s_token
{
	char	*raw_token;
	int		type;
	bool	expand_var;
	char	*final_token;
	
	//struct	s_token	*previous;	
	struct	s_token	*next;	
}			t_token;

// SHELL
typedef struct s_shell
{
	char	*input;
	char	**environment;
	t_token	*token_list;
}			t_shell;

// FUNCTIONS -----------------------------------------------------

// 00_minishell.c
void	start_minishell(char *prompt, char **environment_var);
void	input_parser(t_shell *shell);
char	*input_reader(char *prompt);

// 01_init_shell.c
t_shell *initialize_shell(void);
void	load_environment_variables(t_shell *shell, char **environment_var);

// 02_parser_tokenize.c
void	tokenizer(t_shell *shell);
char	*quotes_tokenizer(char *input, int index_first_char, char delimiter);
char	*word_tokenizer(char *input, int index_first_char);

// 03_parser_list_token.c
void	add_back_token(t_token **token_list, char  *input, int token_type);
void	print_token_list(t_token *token_list);
	
//  04_parser_expand.c
int		search_expand_operators(t_token *token_list);
void	activate_expand_operators(t_token *token_list, char **env);
void	expand_token(t_token *token, char **env);
char	*extract_variable(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
t_expand *initialize_expand_stack(void);

//  05_parser_expand_utils.c
void	print_expand_stack(t_expand *expand_stack);

// 08_utils.c
int		is_operator(char character);
int		is_space(char character);

// 09_free_functions.c
void	cleanup_minishell(t_shell *shell);
void	free_token_list(t_token **token_list);
void	free_expand_stack(t_expand *expand_stack);
void	free_matrix(char **matrix);

// 15_utils_process.c 
void	print_text_file(const char *filename);
void	print_config_shell(t_shell *shell);
void	print_strings_array(char **array);

#endif