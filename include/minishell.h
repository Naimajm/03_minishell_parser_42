/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/14 14:55:37 by juagomez         ###   ########.fr       */
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
# define FAILURE	-1

//# define ERROR_ARGS			"Error\n Error number arguments\n"
# define ERROR_ENVIRONMENT				"Error\n Environment unavailable or empty\n"
# define ERROR_OPEN_FILE				"Error\n Opening file\n"
# define ERROR_INPUT_READER				"Error\n Error read input\n"
# define ERROR_STRUCT_INITIALIZATION	"Error\n structure initialization error\n" 

# define ERROR_QUOTE_SYNTAX	"Error\n Simple/double quote syntax. Not closed\n"

# define PROMPT				"minishell$ "

# define FREE_ALL_SHELL		"Free\n Total cleaning minishell... OK\n"
# define FREE_WORDS_LIST	"Free\n words list... OK\n"
# define FREE_TOKENS		"Free\n cleaning Token List... OK\n"
# define FREE_MATRIX		"Free\n cleaning matrix... OK\n"

// categorizacion WORD_TOKEN
# define WORD					'W'		// W -> palabra generica no expandido
# define OPERATOR				'O'		// O -> operador (< << > >> |)

// categorizacion TOKENS
//# define WORD					1		// 1 -> generico no operador		
# define NO_QUOTES				2		// 2 -> palabras sin comillas
# define SINGLE_QUOTES			3		// 3 -> palabras con comillas simples -> literal
# define DOUBLE_QUOTES			4		// 4 -> palabras con comillas dobles -> expansion variables
# define OUTFILE				5		// 5 -> operador > OUTFILE
# define APPEND					6		// 6 -> operador >> APPEND
# define INFILE					7		// 7 -> operador < INFILE
# define HERE_DOC				8		// 8 -> operador <> HERE_DOC
# define PIPE					9		// 9 -> operador | PIPE

// categorizacion VARIABLES EXPANDIDAS
# define BASIC_EXPANSION		1		// 1 -> expansion basica
# define CURLY_BRACES			2		// 2 -> expansion basica con llaves {}
# define LAST_EXIT_STATUS		3		// 3 -> caso especial $? -> ultimo exit_status
# define LITERAL				4		// 4 -> caso especial \$ -> valor literal

// STRUCT -----------------------------------------------------

// VARIABLES EXPANDIDAS
typedef struct s_expand
{	
	int		type;
	int		first_index;
	int		last_index;
	char	*substitution_str;
	char	*key;
	char	*value;			
	struct	s_expand	*next;	
}			t_expand;

// TOKEN
typedef struct s_token
{
	int			type;
	char		*raw_token;	
	char		*expanded_token;	
	char		*noquotes_token;

	//char		*joined_token;
	//bool		require_join;
	
	t_expand	*expand_list; // lista nodos expansion variables
	
	struct		s_token	*next;	
}				t_token;

// WORD TOKENS
typedef struct s_word_token
{
	char		word_type;
	char		*raw_word;
	char		*processed_word;

	t_token		*tokens_list;  // lista tokens para expansion variables
	struct		s_word_token *next;
	
}				t_word_token;

// SHELL
typedef struct s_shell
{
	char			*input;
	char			**environment;

	int				exit_status;
	int				last_exit_status;

	t_word_token	*words_list;
}			t_shell;

// FUNCTIONS -----------------------------------------------------

// 00_minishell.c
void	start_minishell(char *prompt, char **environment_var);
void	input_parser(t_shell *shell);
char	*input_reader(char *prompt);

// 01_init_shell.c
t_shell *initialize_shell(void);
void	load_environment_variables(t_shell *shell, char **environment_var);

// 02_parser_word.c
void	lexical_analyzer(t_shell *shell);
int		word_extractor(t_shell *shell, int index_first_char);
int		operator_extractor(t_shell *shell, int index_first_char);

// 02.1_parser_word_utils.c
void	add_word_node(t_word_token **word_list, char  *input, char word_type);
void 	print_words_list(t_word_token *word_list);

// 03_parser_tokenize.c
void	tokenizer(t_shell *shell);
int		noquotes_tokenizer(t_shell *shell, int index_first_char);
int		quotes_tokenizer(t_shell *shell, int index_first_char);
int		operator_extractor(t_shell *shell, int index_first_char);

// 03.1_parser_tokenize_utils.c
void	add_token_node(t_token **token_list, char  *input, int token_type);
void	print_tokens_list(t_token *token_list);
	
//  04_parser_expand.c
void	activate_expand_operators(t_shell *shell);
void	generate_expand_list(t_token *token);
void	resolve_expansion_values(t_token *token, t_shell *shell);
void	insert_expansion_values(t_token *token);

//  04.1_parser_expand_list.c
int		basic_expander(t_token *token, int first_index);
int		last_exit_status_expander(t_token *token, int first_index);
int		curly_braces_expander(t_token *token, int first_index);
int		literal_expander(t_token *token, int first_index);

//  04.2_parser_expand_extract.c
char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
char	*extract_substitution_segment(char *input, int index_first_char);

//	04.3_parser_expand_utils.c
t_expand	*add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type);
void 	print_expand_nodes_list(t_expand *expand_list);

//  04.4_parser_dequotize.c
void	join_tokens(t_token *token_list);
void	dequotize(t_token *token_list);
void	remove_quotes(t_token *token);

// 08_utils.c
void	print_message_and_exit(char *message, int fd, int exit_code);
int 	find_index_char(const char *str, char character);
int		is_quote(char character);
int		is_operator(char character);
int		is_space(char character);

// 09_utils_2.c
char	*ft_strjoin_free(char *str1, char *str2);

// 09_free_functions.c
void	cleanup_minishell(t_shell *shell);
void	free_words_list(t_word_token **words_list);
void	free_tokens_list(t_token **token_list);
void	free_expand_stack(t_expand *expand_stack);
void	free_matrix(char **matrix);

// 15_utils_process.c 
void	print_text_file(const char *filename);
void	print_config_shell(t_shell *shell);
void	print_strings_array(char **array);

// 16_testing.c
void test_lexical_analyzer(t_shell *shell);

#endif