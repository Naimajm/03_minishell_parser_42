/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/20 22:35:32 by juagomez         ###   ########.fr       */
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



// ERRORES GENERALES
# define ERROR_ENVIRONMENT				"Error\n Environment unavailable or empty\n"
# define ERROR_OPEN_FILE				"Error\n Opening file\n"
# define ERROR_INPUT_READER				"Error\n Error read input\n"
# define ERROR_ADVANCE_INDEX			"Error\n Index advance error\n"

// ERRORES CONSTRUCCION
# define ERROR_STRUCT_INITIALIZATION	"Error\n Structure initialization error\n" 
# define ERROR_COMMAND_INITIALIZATION	"Error\n Command structure initialization failed\n" 
# define ERROR_WORD_INITIALIZATION		"Error\n Word structure initialization failed\n" 
# define ERROR_TOKEN_INITIALIZATION		"Error\n Token structure initialization failed\n" 
# define ERROR_EXPAND_INITIALIZATION	"Error\n Expand structure initialization failed\n" 

// ERRORES DE SINTAXIS Y PARSING
# define ERROR_QUOTE_SYNTAX	"Error\n Simple/double quote syntax. Not closed\n"

// ERRORES DE VALIDACIÓN
# define ERROR_INVALID_INPUT			"Error\n Invalid input parameter\n"

// ERROR CONSTRUCCION ESTRUCTURAS

/* // ERRORES GENERALES
# define ERROR_ENVIRONMENT				"Error: Environment unavailable or empty\n"
# define ERROR_OPEN_FILE				"Error: Opening file\n"
# define ERROR_INPUT_READER				"Error: Failed to read user input\n"
# define ERROR_MEMORY_ALLOCATION		"Error: Memory allocation failed\n"
# define ERROR_NULL_POINTER				"Error: Null pointer detected\n"

// ERRORES DE SINTAXIS Y PARSING
# define ERROR_QUOTE_SYNTAX				"Error: Unmatched quotes detected\n"
# define ERROR_PIPE_SYNTAX				"Error: Invalid pipe syntax\n"
# define ERROR_REDIRECT_SYNTAX			"Error: Invalid redirection syntax\n"
# define ERROR_COMMAND_EMPTY			"Error: Empty command detected\n"
# define ERROR_INVALID_OPERATOR			"Error: Invalid operator sequence\n"

// ERRORES DE CONSTRUCCIÓN - COMMAND BUILDER
# define ERROR_COMMAND_CREATION			"Error: Failed to create command node\n"
# define ERROR_COMMAND_INITIALIZATION	"Error: Command structure initialization failed\n"
# define ERROR_COMMAND_DUPLICATION		"Error: Failed to duplicate command string\n"
# define ERROR_COMMAND_LIST_CORRUPTED	"Error: Command list structure corrupted\n"

// ERRORES DE CONSTRUCCIÓN - WORD BUILDER  
# define ERROR_WORD_CREATION			"Error: Failed to create word node\n"
# define ERROR_WORD_INITIALIZATION		"Error: Word structure initialization failed\n"
# define ERROR_WORD_DUPLICATION			"Error: Failed to duplicate word string\n"
# define ERROR_WORD_EXTRACTION			"Error: Word extraction failed\n"
# define ERROR_WORD_LIST_CORRUPTED		"Error: Word list structure corrupted\n"

// ERRORES DE CONSTRUCCIÓN - TOKEN BUILDER
# define ERROR_TOKEN_CREATION			"Error: Failed to create token node\n"
# define ERROR_TOKEN_INITIALIZATION		"Error: Token structure initialization failed\n"
# define ERROR_TOKEN_DUPLICATION		"Error: Failed to duplicate token string\n"
# define ERROR_TOKEN_CLASSIFICATION		"Error: Token classification failed\n"
# define ERROR_TOKEN_LIST_CORRUPTED		"Error: Token list structure corrupted\n"

// ERRORES DE CONSTRUCCIÓN - EXPAND BUILDER
# define ERROR_EXPAND_CREATION			"Error: Failed to create expansion node\n"
# define ERROR_EXPAND_INITIALIZATION	"Error: Expansion structure initialization failed\n"
# define ERROR_EXPAND_DUPLICATION		"Error: Failed to duplicate expansion string\n"
# define ERROR_EXPAND_KEY_EXTRACTION	"Error: Failed to extract variable key\n"
# define ERROR_EXPAND_VALUE_RESOLUTION	"Error: Failed to resolve variable value\n"
# define ERROR_EXPAND_LIST_CORRUPTED	"Error: Expansion list structure corrupted\n"

// ERRORES DE PROCESAMIENTO - LEXICAL ANALYZER
# define ERROR_LEXICAL_ANALYSIS			"Error: Lexical analysis failed\n"
# define ERROR_QUOTE_PROCESSING			"Error: Quote processing failed\n"
# define ERROR_OPERATOR_PROCESSING		"Error: Operator processing failed\n"
# define ERROR_WORD_SEGMENTATION		"Error: Word segmentation failed\n"

// ERRORES DE PROCESAMIENTO - TOKENIZER
# define ERROR_TOKENIZATION				"Error: Tokenization process failed\n"
# define ERROR_QUOTE_TOKENIZATION		"Error: Quote tokenization failed\n"
# define ERROR_OPERATOR_TOKENIZATION	"Error: Operator tokenization failed\n"
# define ERROR_NOQUOTES_TOKENIZATION	"Error: No-quotes tokenization failed\n"

// ERRORES DE PROCESAMIENTO - VARIABLE EXPANDER
# define ERROR_VARIABLE_EXPANSION		"Error: Variable expansion failed\n"
# define ERROR_EXPANSION_DETECTION		"Error: Expansion detection failed\n"
# define ERROR_VARIABLE_RESOLUTION		"Error: Variable resolution failed\n"
# define ERROR_VALUE_INSERTION			"Error: Value insertion failed\n"
# define ERROR_EXIT_STATUS_EXPANSION	"Error: Exit status expansion failed\n"
# define ERROR_CURLY_BRACES_EXPANSION	"Error: Curly braces expansion failed\n"
# define ERROR_LITERAL_EXPANSION		"Error: Literal expansion failed\n"

// ERRORES DE PROCESAMIENTO - DEQUOTIZER
# define ERROR_DEQUOTIZATION			"Error: Quote removal failed\n"
# define ERROR_QUOTE_REMOVAL			"Error: Quote character removal failed\n"

// ERRORES DE PROCESAMIENTO - WORD PROCESSOR
# define ERROR_WORD_PROCESSING			"Error: Word processing failed\n"
# define ERROR_TOKEN_JOINING			"Error: Token joining failed\n"
# define ERROR_PROCESSED_WORD_CREATION	"Error: Processed word creation failed\n"

// ERRORES DE CONSTRUCCIÓN - EXECUTION BUILDER
# define ERROR_EXECUTION_BUILD			"Error: Execution structure build failed\n"
# define ERROR_ARGS_EXTRACTION			"Error: Arguments extraction failed\n"
# define ERROR_REDIRECTIONS_EXTRACTION	"Error: Redirections extraction failed\n"
# define ERROR_BUILTIN_DETECTION		"Error: Builtin command detection failed\n"

// ERRORES DE AVANCE DE ÍNDICE
# define ERROR_ADVANCE_INDEX			"Error: Index advance calculation failed\n"
# define ERROR_INDEX_OUT_OF_BOUNDS		"Error: Index out of bounds\n"
# define ERROR_INVALID_LENGTH			"Error: Invalid length parameter\n"

// ERRORES DE VALIDACIÓN
# define ERROR_INVALID_INPUT			"Error: Invalid input parameter\n"
# define ERROR_EMPTY_INPUT				"Error: Empty input detected\n"
# define ERROR_CORRUPTED_STRUCTURE		"Error: Data structure corrupted\n"
# define ERROR_INCONSISTENT_STATE		"Error: Inconsistent program state\n" */


# define PROMPT				"minishell$ "

# define FREE_ALL_SHELL		"Free\n Total cleaning minishell... OK\n"
# define FREE_COMMANDS_LIST	"Free\n Commands list...\t\t\t OK\n"
# define FREE_WORDS_LIST	"Free\n Words list... OK\n"
# define FREE_TOKENS_LIST	"Free\n Tokens List... OK\n"
# define FREE_EXPANDS_LIST	"Free\n Expands List... OK\n"
# define FREE_MATRIX		"Free\n Cleaning matrix... OK\n"

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
	
	t_expand	*expands_list; 			// lista nodos expansion variables
	
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
	char		*command;		// NUEVO -> trozo input perteneciente a este proceso

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

	t_cmd		*commands_list;
}				t_shell;

// FUNCTIONS -----------------------------------------------------

/// ARCHIVOS PRINCIPALES -------------------------------------------
// 00_main.c 				# Función main y loop principal
int		main(int	argc, char **argv, char **environment);
void	run_shell(t_shell *shell);
void	process_input(t_shell *shell);
void	process_commands(t_shell *shell);
char	*read_user_input(char *prompt);

// 01_shell_init.c			# Inicialización del shell
int		validate_environment(char **environment);
t_shell *initialize_shell(void);
int		load_environment_variables(t_shell *shell, char **environment);

/// ANÁLISIS SINTÁCTICO -------------------------------------------
// 02_syntax_analyzer.c		# Análisis sintáctico inicial
void	syntax_analyzer(t_shell *shell);
void	create_commands_structure(t_shell *shell);
int 	generate_command(t_shell *shell, int start_index);
char	*create_clean_command(char *input, int start_index, int final_index);
int 	is_pipe_operator(char character);

// 02.1_command_builder.c	# Constructor de comandos
void	add_command_node(t_cmd **commands_list, char *input);
void	print_commands_list(t_cmd *commands_list);

/// ANÁLISIS LÉXICO -------------------------------------------
// 03_lexical_analyzer.c	# Análisis léxico
void	lexical_analyzer(t_cmd *commands_list);
void	command_extractor(t_cmd *command);
int		word_extractor(t_cmd *command, int start_index);
int		operator_extractor(t_cmd *command, int start_index);

// 03.1_word_builder.c		# Constructor de palabras
void	add_word_node(t_word **word_list, char  *input, char word_type);
void 	print_words_list(t_word *word_list);

/// TOKENIZACIÓN -------------------------------------------
// 04_tokenizer.c			# Tokenización
void	tokenizer(t_word *words_list);
void	word_tokenizer(t_word *word);
int		noquotes_tokenizer(t_word *word, int start_index);
int		quotes_tokenizer(t_word *word, int start_index);
int		operator_tokenizer(t_word *word, int start_index);

// 04.1_token_builder.c		# Constructor de tokens
void	add_token_node(t_token **token_list, char  *input, int token_type);
void	print_tokens_list(t_token *token_list);

/// EXPANSIÓN DE VARIABLES -------------------------------------------
//  05_variable_expander.c	# Expansión de variables
void	variable_expander(t_word *words_list, char **environment, int exit_status);
void	expand_single_token(t_token *token, char **environment, int exit_status);
void	extract_expansion_nodes(t_token *token);
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
char	*extract_substitution_segment(char *input, int start_index);

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
int		advance_index_by_length(int current_index, int length);
void	perror_exit(char *message, int fd, int exit_code);
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