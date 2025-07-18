/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/18 13:35:26 by juagomez         ###   ########.fr       */
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
# define FREE_COMMANDS_LIST	"Free\n Commands list... OK\n"
# define FREE_WORDS_LIST	"Free\n Words list... OK\n"
# define FREE_TOKENS		"Free\n Token List... OK\n"
# define FREE_MATRIX		"Free\n cleaning matrix... OK\n"

// categorizacion TIPOS WORD_TOKEN
# define WORD					'W'		// W -> palabra generica no expandido (NO_QUOTES, SINGLE_QUOTES, DOUBLE_QUOTES)
# define OUTFILE				'O'		// 1 -> operador > OUTFILE
# define APPEND					'A'		// 2 -> operador >> APPEND
# define INFILE					'I'		// 3 -> operador < INFILE
# define HERE_DOC				'H'		// 4 -> operador << HERE_DOC
# define PIPE					'P'		// 5 -> operador | PIPE

// categorizacion TOKENS
//# define WORD					1		// 1 -> generico no operador		

# define NO_QUOTES				2		// 2 -> palabras sin comillas
# define SINGLE_QUOTES			3		// 3 -> palabras con comillas simples -> literal
# define DOUBLE_QUOTES			4		// 4 -> palabras con comillas dobles -> expansion variables
# define OPERATOR				5		// 5 -> operador (< << > >> |)

/* # define OUTFILE				5		// 5 -> operador > OUTFILE
# define APPEND_OPERATOR		6		// 6 -> operador >> APPEND
# define INFILE					7		// 7 -> operador < INFILE
# define HERE_DOC				8		// 8 -> operador <> HERE_DOC
# define PIPE					9		// 9 -> operador | PIPE */

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
	
	t_expand	*expand_list; 			// lista nodos expansion variables
	
	struct		s_token	*next;	
}				t_token;

// WORD TOKENS
typedef struct s_word
{
	char		word_type;
	char		*raw_word;
	char		*processed_word;

	t_token		*tokens_list;  			// lista tokens para expansion variables
	
	struct		s_word *next;	
}				t_word;

// LISTA COMANDOS
/* typedef	struct 	s_command
{
	char		*chunk_input;		// NUEVO -> trozo input perteneciente a este proceso

	char		**args;				// Array de argumentos

	char		*infile;			// Archivo de entrada	
	char		*outfile;			// Archivo de salida
	char		*heredoc_delimiter;	// heredoc_delimiter
	
	char		redirect_mode;  	// NUEVO !! Modo append (>>),  Modo heredoc (<<), Modo infile (<), Modo outfile (>), Pipe (|)	
	
	bool		is_builtin;		// flag si el comando es builtin
	int			exit_status;

	t_word *words_list;	// NUEVO LISTA ASOCIADA DE PALABRAS + OPERADORES30.

	struct 		s_command	*next;
}				t_command; */

typedef	struct 	s_cmd
{
	char		*chunk_input;		// NUEVO -> trozo input perteneciente a este proceso

	char		**args;				// Array de argumentos

	char		*infile;			// Archivo de entrada	
	char		*outfile;			// Archivo de salida
	char		*delimiter;			// heredoc_delimiter

	bool		append;				// modo append
	bool		hd;					// modo heredoc
	bool		is_btn;				// flag si el comando es builtin
	int			exit_status;

	t_word *words_list;				// NUEVO LISTA ASOCIADA DE PALABRAS + OPERADORES30.

	struct 		s_cmd	*next;
}				t_cmd; 

// SHELL
typedef struct	s_shell
{
	char		*input;
	char		**environment;

	int			exit_status;
	int			last_exit_status;

	t_cmd		*command_list;
}				t_shell;

// FUNCTIONS -----------------------------------------------------

/// ARCHIVOS PRINCIPALES -------------------------------------------
// 00_main.c 				# Función main y loop principal
void	start_minishell(char *prompt, char **environment_var);
void	process_comands(t_shell *shell);
void	input_parser(t_shell *shell);
char	*input_reader(char *prompt);

// 01_shell_init.c			# Inicialización del shell
t_shell *initialize_shell(void);
void	load_environment_variables(t_shell *shell, char **environment_var);

/// ANÁLISIS SINTÁCTICO -------------------------------------------
// 02_syntax_analyzer.c		# Análisis sintáctico inicial
void	syntax_analyzer(t_shell *shell);
void	create_commands_structure(t_shell *shell);
int 	is_pipe(char character);

// 02.1_command_builder.c	# Constructor de comandos
void	add_command_node(t_cmd **commands_list, char *input);
void	print_commands_list(t_cmd *commands_list);

/// ANÁLISIS LÉXICO -------------------------------------------
// 03_lexical_analyzer.c	# Análisis léxico
void	lexical_analyzer(t_cmd *process_list);
int		word_extractor(t_cmd *process_list, int index_first_char);
int		operator_extractor(t_cmd *process_list, int index_first_char);

// 03.1_word_builder.c		# Constructor de palabras
void	add_word_node(t_word **word_list, char  *input, char word_type);
void 	print_words_list(t_word *word_list);

/// TOKENIZACIÓN -------------------------------------------
// 04_tokenizer.c			# Tokenización
void	tokenizer(t_word *words_list);
int		noquotes_tokenizer(t_word *word, int index_first_char);
int		quotes_tokenizer(t_word *word, int index_first_char);
int		operator_tokenizer(t_word *word, int index_first_char);

// 04.1_token_builder.c		# Constructor de tokens
void	add_token_node(t_token **token_list, char  *input, int token_type);
void	print_tokens_list(t_token *token_list);

/// EXPANSIÓN DE VARIABLES -------------------------------------------
//  05_variable_expander.c	# Expansión de variables
void	activate_expand_operators(t_word *words_list, char **environment, int exit_status);
void	generate_expand_list(t_token *token);
void 	resolve_expansion_values(t_token *token, char **environment, int exit_status);
void	insert_expansion_values(t_token *token);

//  05.1_expand_list.c		# Lista de expansiones
int		basic_expander(t_token *token, int first_index);
int		last_exit_status_expander(t_token *token, int first_index);
int		curly_braces_expander(t_token *token, int first_index);
int		literal_expander(t_token *token, int first_index);

//  05.2_expand_extractor.c	# Extractor de variables
char	*extract_key(char *token, int first_index);
char	*get_environment_var(char **env, char *variable);
char	*extract_substitution_segment(char *input, int index_first_char);

//	05.3_expand_builder.c	 # Constructor de expansiones
t_expand	*add_expand_node(t_expand **expand_list, char  *substitution_variable, int first_index, int expand_type);
void 	print_expand_nodes_list(t_expand *expand_list);

//  05.4_dequotizer.c		# Eliminación de comillas
void	dequotize_tokens(t_word *words_list);
void	remove_quotes(t_token *token);

/// PROCESAMIENTO FINAL -------------------------------------------
// 06_word_processor.c		
void	generate_processed_word(t_word **words_list);
void	insert_token_node(t_word *word);

// 07_execution_builder.c 
void    build_execution_structure(t_cmd *commands_list);
void    extract_command_arguments(t_cmd *command);
void    extract_redirections(t_cmd *command);
int     count_word_arguments(t_word *words_list);
bool 	is_builtin_command(char *command);

/// UTILIDADES Y TESTING -------------------------------------------
// 10_utils_core.c			 # Utilidades básicas
int 	find_index_char(const char *str, char character);
int		is_quote(char character);
int		is_operator(char character);
int		is_space(char character);

// 10.1_utils_strings.c
void	print_message_and_exit(char *message, int fd, int exit_code);
char	*ft_strjoin_free(char *str1, char *str2);
	
// 10.2_free_manager.c		# Gestión de memoria
void	cleanup_minishell(t_shell *shell);
void	free_commands_list(t_cmd **commands_list);
void	free_words_list(t_word **words_list);
void	free_tokens_list(t_token **token_list);
void	free_expands_list(t_expand *expand_stack);

void	free_matrix(char **matrix);  // TODO -> LLEVAR A NUEVO ARCHIVO

// 10.3_utils_debug.c 		# Utilidades de debug
void	print_text_file(const char *filename);
void	print_config_shell(t_shell *shell);
void	print_strings_array(char **array);

// 11_testing.c				# Testing y debugging
void	test_lexical_analyzer(t_shell *shell);

#endif