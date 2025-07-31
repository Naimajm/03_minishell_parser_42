/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:56:38 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 19:28:40 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// OWN LIBRARIES
# include "../lib/libft/libft.h"
# include "../lib/gnl/get_next_line.h"

// EXTERNAL LIBRARIES PARSER
#include <unistd.h>		// write getcwd chdir
#include <stdio.h>  	// printf aux
#include <fcntl.h>		// open
#include <stdbool.h>	// bool

#include <readline/readline.h> 	// readline
#include <readline/history.h>	// readline

// EXTERNAL LIBRARIES BUILTINS
#include <stdlib.h>	// exit 

// EXTERNAL LIBRARIES EXECUTOR
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>

// MACROS EXECUTOR -------------------------------------------------

// # define WORD 0 // TODO, DEFINIDOS EN MACROS PARSER
// TODO, DEFINIDOS EN MACROS PARSER CON OTROS NOMBRES
// # define DQ 2
// # define PI 3
// # define RR1 4
// # define RR2 5
// # define LR1 6
// # define LR2 7
# define LOL 999

// extern int	g_signal_flag; // TODO, DEFINIDOS EN MACROS PARSER

// MACROS PARSER-----------------------------------------------------
// VALORES EXIT_STATUS
# define FAILURE			-1
# define SUCCESS			0
# define ERROR				1
# define SYNTAX_ERROR		2

# define COMMAND_NOT_FOUND	127
# define EXIT_SIGINT		130		// Ctrl+'C'
# define EXIT_SIGQUIT       131    	// Ctrl+'\'

// ERRORES GENERALES
# define ERROR_ENVIRONMENT				"Error\n Environment unavailable or empty\n"
# define ERROR_OPEN_FILE				"Error\n Opening file\n"
# define ERROR_INPUT_READER				"Error\n Error read input\n"
# define ERROR_CHECK_SYNTAX				"Error\n Check syntax input Error\n"
# define ERROR_ADVANCE_INDEX			"Error\n Index advance error\n"

// ERRORES DE SINTAXIS Y PARSING
# define ERROR_QUOTE_SYNTAX				"Error\n Syntax error: Unmatched quotes detected\n"
# define ERROR_REDIRECTION_SYNTAX		"Error\n Syntax error: Invalid redirection syntax\n"
# define ERROR_PIPE_SYNTAX				"Error\n Syntax error: Invalid pipe syntax\n"

// ERRORES CONSTRUCCION
# define ERROR_INIT						"Error\n Initialization error\n" 
# define ERROR_COMMAND_INIT				"Error\n Command structure initialization failed\n" 
# define ERROR_WORD_INIT				"Error\n Word structure initialization failed\n" 
# define ERROR_TOKEN_INIT				"Error\n Token structure initialization failed\n" 
# define ERROR_EXPAND_INIT				"Error\n Expand structure initialization failed\n" 

// ERRORES DE VALIDACIÓN
# define ERROR_INVALID_INPUT			"Error\n Invalid input parameter\n"
# define ERROR_MEMORY_ALLOC				"Error\n Memory allocation failed\n"
# define ERROR_COMMAND_EMPTY			"Error\n Empty command detected\n"

# define PROMPT					"minishell$ "

# define FREE_ALL_SHELL			"Free\n Total cleaning minishell... OK\n"
# define FREE_ITERATION_INPUT	"Free\n Iteration Input...\t\t\t OK\n"
# define FREE_COMMANDS_LIST		"Free\n Commands list...\t\t\t OK\n"
# define FREE_WORDS_LIST		"Free\n Words list...\t\t\t\t OK\n"
# define FREE_TOKENS_LIST		"Free\n Tokens List...\t\t\t\t OK\n"
# define FREE_EXPANDS_LIST		"Free\n Expands List...\t\t\t OK\n"
# define FREE_MATRIX			"Free\n Cleaning matrix...\t\t\t OK\n"

// categorizacion TIPOS WORD_TOKEN
# define WORD					'W'		// W -> palabra generica no expandido (NO_QUOTES, SINGLE_QUOTES, DOUBLE_QUOTES)
# define OUTFILE				'O'		// 1 -> operador > OUTFILE
# define APPEND					'A'		// 2 -> operador >> APPEND
# define INFILE					'I'		// 3 -> operador < INFILE
# define HERE_DOC				'H'		// 4 -> operador << HERE_DOC
# define PIPE					'P'		// 5 -> operador | PIPE

// categorizacion TOKENS
# define NO_QUOTES				1		// 1 -> palabras sin comillas
# define SINGLE_QUOTES			2		// 2 -> palabras con comillas simples -> literal
# define DOUBLE_QUOTES			3		// 3 -> palabras con comillas dobles -> expansion variables
# define OPERATOR				4		// 4 -> operador (< << > >> |)

// categorizacion VARIABLES EXPANDIDAS
# define BASIC_EXPANSION		1		// 1 -> expansion basica
# define CURLY_BRACES			2		// 2 -> expansion basica con llaves {}
# define LAST_EXIT_STATUS		3		// 3 -> caso especial $? -> ultimo exit_status
# define LITERAL				4		// 4 -> caso especial \$ -> valor literal

// STRUCT -----------------------------------------------------

// GLOBAL VARIABLE
extern int	g_signal_flag;

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
	
	t_expand	*expands_list; 			
	
	struct		s_token	*next;	
}				t_token;

// WORD TOKENS
typedef struct s_word
{
	char		word_type;
	char		*raw_word;
	char		*processed_word;

	t_token		*tokens_list;  			
	
	struct		s_word *next;	
}				t_word;

typedef	struct 	s_cmd
{
	char		*command;			// NUEVO -> trozo input perteneciente a este proceso
	char		**args;				// Array de argumentos

	char		*infile;			// Archivo de entrada	
	char		*outfile;			// Archivo de salida
	char		*delimiter;			// heredoc_delimiter

	bool		append;				// modo append
	bool		hd;					// modo heredoc
	bool		is_btn;				// flag si el comando es builtin
	int			exit_status;

	t_word 		*words_list;		// NUEVO LISTA ASOCIADA DE PALABRAS + OPERADORES30.

	struct 		s_cmd	*next;
}				t_cmd; 

// SHELL
typedef struct	s_shell
{
	char		*input;
	char		**environment;

	int			exit_status;
	int			last_exit_status;

	char		**readonly_vars; // AÑADIDO POR EMI PARA LA GESTION DE VARIABLE DE SOLO LECTURA DE LA BUILTIN UNSET

	t_cmd		*commands_list;
}				t_shell;

// FUNCTIONS -----------------------------------------------------

//SRC------------------------------------------------
// 00_main.c 				# Función main 
int		main(int	argc, char **argv, char **environment);
int		validate_environment(char **environment);
t_shell *initialize_shell(void);
int		load_environment_variables(t_shell *shell, char **environment);
void	cleanup_minishell(t_shell *shell);

// 01_execute_shell.c			# loop principal Shell
void	execute_shell(t_shell *shell);
void	recover_previous_status(t_shell *shell);
void	read_user_input(t_shell *shell, char *prompt);
void	process_input(t_shell *shell);
void	process_commands(t_shell *shell);

// END SRC----------------------------------------------

/// PARSER -------------------------------------------
// 01.1_check_syntax.c		# Validacion sintaxis input
int		validate_syntax(t_shell *shell);
int		check_pipe_syntax(char *input);
int		check_balanced_quotes(char *input);
int		check_redirection_syntax(char *input);

/// ANÁLISIS SINTÁCTICO -------------------------------------------
// 02_command_generate.c	# Análisis sintáctico inicial
void	create_commands_structure(t_shell *shell);
int 	generate_command(t_shell *shell, int start_index);
char	*create_clean_command(char *input, int start_index, int final_index);

// 02.1_command_check.c	# Validacion sintaxis estructura comandos
int			validate_command_structure(t_shell *shell);
int 		validate_pipe_structure(t_cmd *commands_list);
int 		validate_redirections_structure(t_cmd *commands_list);

// 02.2_command_builder.c	# Constructor de comandos
void	add_command_node(t_cmd **commands_list, char *input);
void	print_commands_list(t_cmd *commands_list);
void	print_command_arguments(char **args);
void	print_output(char **args);

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
void	tokenizer(t_word *words_list, t_shell *shell);
int		word_tokenizer(t_word *word);
int		noquotes_tokenizer(t_word *word, int start_index);
int		quotes_tokenizer(t_word *word, int start_index);
int		operator_tokenizer(t_word *word, int start_index);

// 04.1_token_builder.c		# Constructor de tokens
void	add_token_node(t_token **token_list, char  *input, int token_type);
void	print_tokens_list(t_token *token_list);

/// EXPANSIÓN DE VARIABLES -------------------------------------------
//  05_variable_expander.c	# Expansión de variables
void	variable_expander(t_word *words_list, t_shell *shell);
int		expand_single_token(t_token *token, t_shell *shell);
void	extract_expansion_nodes(t_token *token);
void 	resolve_expansion_values(t_token *token, t_shell *shell);
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
void	dequotize_tokens(t_word *words_list, t_shell *shell);
int		remove_quotes(t_token *tokens_list);

/// PROCESAMIENTO FINAL -------------------------------------------
// 06_word_processor.c		
void	generate_processed_word(t_word *words_list, t_shell *shell);
int		insert_token_node(t_word *word);

// 07_semantic_check.c
int 		validate_command_semantics(t_shell *shell);

// 08_execution_builder.c 
void    build_execution_structure(t_cmd *commands_list);
void    extract_command_arguments(t_cmd *command);
void    extract_redirections(t_cmd *command);
int     count_word_arguments(t_word *words_list);
bool 	is_builtin_command(char *command);

/// UTILIDADES Y TESTING -------------------------------------------
// 10_utils_core.c			 # Utilidades básicas
int 	find_index_char(const char *str, char character);
int 	is_pipe(char character);
int		is_quote(char character);
int		is_operator(char character);
int		is_space(char character);

// 10.1_utils_strings.c
int		advance_index_by_length(int current_index, int length);
char	*ft_strjoin_free(char *str1, char *str2);
void	free_matrix(char **matrix);
	
// 10.2_free_manager.c		# Gestión de memoria
void	free_iteration_input(t_shell *shell);
void	free_commands_list(t_cmd **commands_list);
void	free_words_list(t_word **words_list);
void	free_tokens_list(t_token **token_list);
void	free_expands_list(t_expand **expands_list);

// 10.3_utils_debug.c 		# Utilidades de debug
void	print_text_file(const char *filename);
void	print_config_shell(t_shell *shell);
void	print_strings_array(char **array);

// 12_test_parser.c			# Testing y debugging
void 	test_basic_parser(t_shell *shell);
void 	test_complex_parser(t_shell *shell);

// ------------------------------------------------------
// END PARSER-----------------------------------------


// EXECUTOR------------------------------------------------------------
// CLEAN.C // TODO, ESTAS FUNCIONES YA ESTAN EN LA PARTE DE PARSEO , AUNQUE CON MODIFICACION DE LOS NOMBRES. HABRIA QUE ADAPTAR LAS LLAMADAS A ELLAS EN LA PARTE DE EXECUTOR

void	ft_free_matrix(char **matrix);
void	ft_free_tknlst(t_token **token);
void	ft_free_cmdlst(t_cmd **cmd_lst);
void	ft_cleanup_shell(t_shell *shell);
void	ft_clean(char **matrix, t_token *token, t_shell *shell);

// CLEAN2.C

void	ft_free_cmd_args(t_cmd *cmd);
void	ft_free_cmd_files(t_cmd *cmd);

// COMMANDS_UTILS.C

int		ft_strcmp(char *s1, char *s2);
int		ft_count_args(t_token *token);
int		ft_isbuiltin(char *str);
int		ft_has_commands(t_shell *shell);

// EXEC_BUILTINS.C

void	execute_builtin(t_shell *ms, t_cmd *cmd, int prevfd);

// EXECUTER_COMMAND.C

void	free_paths(char **paths, int i);
char	*ft_search_in_paths(char **paths, char *comm);
char	*ft_path(char *path, char **comm);
void	ft_handle_command_execution(t_shell *shell, t_cmd *cmd, char *path);
void	execute_command(t_shell *shell, t_cmd *cmd);

// EXECUTER.C

void	child_process(t_cmd *cmd, int prevfd, int pipefd[2], t_shell *ms);
void	parent_process(t_shell *ms, int *prevfd, int pipefd[2]);
void	ft_wait_all_processes(pid_t *pids, t_shell *ms);
pid_t	ft_exec_single_cmd(t_cmd *cmd, int *prevfd, int *pipefd, t_shell *ms);
void	ft_exec_commands(t_shell *ms);


// GET_COMMANDS.C //

int		ft_cmdsize(t_cmd *lst);
void	ft_fill_cmd(t_shell *shell, t_token *token, t_token **next_token);
void	ft_process_token(t_shell *shell, t_token **temp);
int		ft_has_commands(t_shell *shell);
int		ft_get_commands(t_shell *shell);

// HEREDOC_UTILS.C

char	*ft_expand_variable(int *i, char *buffer, char **env, int exit_st);
char	*ft_not_expand(int *i, char *buffer);
char	*ft_expand_heredoc(char *buffer, char **env, int exit_st);


// PROCESS_REDIRECTION.C //

void	ft_process_redir(t_shell *shell, t_token *token, t_token **next_token);
void	ft_process_input(t_cmd *cmd, t_token *token);
void	ft_process_output(t_cmd *cmd, t_token *token);
void	ft_process_append(t_cmd *cmd, t_token *token);
void	ft_process_heredoc(t_cmd *cmd, t_token *token);

// REDIRECTIONS.C

int		ft_redir_heredoc(t_shell *shell, t_cmd *cmd);
int		ft_redir_infile(char *infile);
int		ft_redir_outfile(char *outfile, int append);
int		ft_redirections(t_shell *shell, t_cmd *cmd);

// SIGNALS.C

void	ft_check_exitstat(int status, t_shell *ms);
void	ft_handle_backslash(int signum);
void	ft_handle_sigint(int signum);
void	ft_setup_signals(void);

// ------------------------------------------------------
// END EXECUTOR-----------------------------------------



// BUILTINS-------------------------------------------------------
//00_utils_builtins_00.c
int		ft_matrix_size(char **mtrx); // unset, export
int		ft_search_index_env(char **env, char str); // export, unset
int		ft_valid_env_var(char *env_var); // export, unset
char	**ft_copy_mtrx(char **mtrx); // export,
void	ft_swap_mtrx(char **s1, char **s2); // export
//00_utils_builtins_01.c
int		ft_strcmp(char *s1, char *s2);
char	*ft_find_plus_pos(char *var); // export
//00_utils_builtins_02.c
char 	**ft_create_new_env(char **env, char *var); // export
//00_utils_builtins_03.c
char	**ft_append_env_var_value(char **env, char *var, int index); //export
//00_exec_builtins.c
void	exec_builtins(t_shell *shell, t_cmd *cmd, int prev_fd);
//btn----one file por each one------------------------------------
int		exec_echo(t_cmd *cmd);
int		exec_cd(t_cmd *cmd, t_shell *shell);
int		exec_pwd(void);
int		exec_export(t_cmd *cmd, t_shell *shell);
int		exec_unset(t_shell *shell, t_cmd *cmd);
int		exec_env(t_shell *shell);
void	exec_exit(t_shell *shell, t_cmd *cmd, int prev_fd);
// END BUILTINS-------------------------------------------------------






#endif