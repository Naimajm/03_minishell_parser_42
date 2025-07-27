/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12_test_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:28:30 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/27 23:44:41 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/wait.h>

#include <string.h>  // strok

void test_complex_parser(t_shell *shell);
void test_basic_parser(t_shell *shell);
static char **get_bash_args_real(char *input);
//static char **get_bash_args(char *input);
static bool compare_args_arrays(char **minishell_args, char **bash_args);
static void print_args_array(char **args, const char *label);

// Casos de prueba enfocados en argumentos
char *test_cases[] = {
	// ECHO TESTS (básicos primero)
	"echo",
	"echo hello",
	"echo hello world",
	"echo hola mundo",
	"echo """,
	"echo "" hola",
	"echo -n hola",
	"echo -n -n hola",
	"echo -n -nh -n hola",
	"echo -nnnnnnnnn hola",
	"echo $?hcudshfuewrf ew  wr 4ew""""",

	// ECHO TESTS - Casos complejos con comillas mixtas
	"echo \"\"uhjkhnkj\"hjbmb\"'''' defdjhfkjb 					\"iujiouhj\"f'f'f'f'f''f'f'f'f'f'f'",
	"echo \"\"uhjkhnkj\"hjbmb\"'''' defdjhfkjb \"iujiouhj\"f'f'a'j'o''i'w'q''q",
	"echo \"\"aaaaaaaa\"bbbbb\"'''' ccccccccc \"dddddddd\"e'e'e",

	// ECHO TESTS - Casos con variables de entorno
	"echo \"ho   $USER\"  'la   $LESS'   $USER$TERM   ",
	"echo \"ho   $USER\"'la   $LESS'$USER$TERM",			
	// ECHO TESTS - Casos sin espacios
	"echo\"ho   $USER\"",
	"echo\"ho   $USE$LESS\"",

	// ECHO TESTS - Comando entre comillas
	"\"echo ho   $USER\"",
	"\"echo\" \"ho   $USER\"",
	"e\"cho\" \"ho   $USER\"",

	// ECHO TESTS - Casos adicionales con variables
	"echo \"ho   $USER\"",
	"echo \"$HOME '$LESS' $USER\"",
	"echo '$HOME \"$LESS\" $USER'",						// error?
	"echo \"$HOME '$LESS \"pa $TERM\" na' $USER\"",		// error ?
	"echo '$HOME \"$LESS 'pa $TERM' na\" $USER'",		// error ?
	"echo '$'\"HOME\"",

	// COMANDOS BÁSICOS
	"echo ls -la /home",
	"echo cat file.txt",
	"echo grep pattern file.txt",
	"echo pwd",
	"echo env",

	// EXPORT TESTS - Casos básicos
	"echo export a=b",
	"echo export b=wawa", 
	"echo export b=lala",
	"echo export c=lala=kaka",
	"echo export a",
	"echo export a=",
	"echo export a= b=o",
	"echo export a===",
	"echo export a3=l",
	"echo export 2a=lala",
	"echo export !a=ma",
	
	// EXPORT TESTS - Con variables de entorno
	"echo export $USER=la",
	"echo export $USER=$TERM",
	"echo export $USER=$TERM$HOME",
	"echo export $USER=$TERM$HOM",
	"echo export $HOME=$USER",
	//"echo export $USER$SHLVL",  // esta ok. cambia el valor numero de  $SHLVL
	
	// EXPORT TESTS - Casos especiales
	"echo export",
	"           echo export",
	//"echo export \"\"",			// esta ok. bash no genera ""
	"echo export a b=$LESS $USER $HOME=la $COLORTERM=pa c=d",

	// CD TESTS
	"echo cd /home",                    // directorio existe
	"echo cd /nonexistent",             // directorio no existe  
	"echo cd /etc/passwd",              // file existe
	"echo cd",                          // cd solo
	"           echo cd",               // cd con espacios
	"echo cd -",                        // cd - = $OLDPWD
	"echo cd $HOME",                    // cd $HOME (si $HOME no existe)
	"echo cd /root",                    // directorio con distintos permisos
	
	// ENV TESTS
	"echo env",                         // env solo
	"echo env lalal",                   // env con argumento inválido
	"echo env /home",                   // env con directorio
	
	// PWD TESTS
	"echo pwd",                         // pwd sin argumentos
	"echo pwd arg1",                    // pwd con argumentos (inválido)
	"echo pwd arg1 arg2",               // pwd con múltiples argumentos
	
	// UNSET TESTS
	"echo unset",                       // unset sin argumentos
	"echo unset PATH",                  // unset con un argumento
	"echo unset PATH HOME USER",        // unset con varios argumentos
	"echo unset NONEXISTENT_VAR",       // unset variable inexistente
	
	// EXIT TESTS
	"echo exit",                        // exit sin argumentos
	"echo exit 0",                      // exit con código 0
	"echo  exit 1",                      // exit con código 1
	"echo exit 42",                     // exit con código 42
	"echo exit lalal",                  // exit con argumento inválido
	"echo exit 256",                    // exit con código > 255
	"echo exit -1",                     // exit con código negativo
	
	// PIPE TESTS
	"echo ls wc",                     // pipe básico
	"echo lswc",                       // pipe sin espacios
	"echo sort colors.txt uniq sort -r  wc",  // pipe múltiple
	"echo ls  wc",                    // double pipe (OR lógico)
	"echo ls    wc",                 // pipe con comando vacío
	"echo ls     wc",         // pipe con redirecciones mezcladas
	"echo ls  wc ",                   // pipe terminando en pipe
	"echo ",                           // solo pipe
	"echo  ls  wc",                   // pipe al inicio

	// REDIRECTION TESTS - Casos básicos 	--------------------------------
	"echo hola  file",                 // Output redirection básica
	"echo hola file",                      // Sin espacios
	"echo wc  colors.txt",                     // Input redirection básica
	"echo wc  colors.txt  file",              // Input y output combinadas
	"echo cat colors.txt  wc",                 // Pipe simple (referencia)
	
	// REDIRECTION TESTS - Casos múltiples
	"echo hola  file  file2  file3",    // Múltiples outputs
	"echo hola  file  file2  file3  ls", // Múltiples outputs + pipe
	
	// REDIRECTION TESTS - Heredoc básicos
	"echo cat  EOF",                          // Heredoc simple
	"echo hola  file  EOF",             // Output + heredoc
	"echo cat  EOF  file",                   // Heredoc + output
	"echo cat  EOF  ls",                     // Heredoc + pipe
	
	// REDIRECTION TESTS - Heredoc múltiples
	"echo cat  hola  que  tal",           // Múltiples heredoc

	// REDIRECTION TESTS - Pipes complejos
	"echo hola  cat  cat  cat  cat  cat  cat  cat", // Pipe muy largo	

	// INPUTS PERSONALIZADOS SIN OPERADORES	
	//"echo \"cat  ho\"la a'$e\" ta'l   ,qu'e\" hac\"$\"es ",	
	"echo 'ho<<$la' 'cara<>|cola'  'ey '  ",
	"  \"ho<< $? la\" \"car$a<>|cola\"  ' ey'  ", 
	" h\"ola $HOME \"caracola  ",	
	" \"aa $HOME $? bb ${HOME}xxx zz\" ",	

	// CASOS ESPECIALES
	" \" $ \"",
	"'$ '",
	"\"$  $HOME\"",
	
	"\" $ $\"",				
	"'$?'",
	"$ \" $ \"",
	"\"$ $ \"",
	"\"$ $\"",	
	"\"$ff{HOME}ff\"",
	"\"ff${HOME}ff\"",
	"xx\"aaa$ezzz\"xx",

	// 2 VARIABLES CONSECUTIVAS
	"$HOME$SHELL", 
	"\"$HOME$SHELL\"",
	"\"zz$HOME$SHELL  xx\"",
	"$SHELL$TERM",
	"$SHELL$XXXX",
	"$HOME=la",

	// LITERALES
	/* "cc\"\\$literal\"",
	"\" aa\"\\$a\\$b\"xx \"",
	"dd\"\\\\$USER\"",
	"ee\"\\\"\\$test\\\"\"",
	"aa\"$HOME\"zz   bb\"$?\"yy   cc\"\\$literal\"  \"${HOME}xxx\"ww",
	"'aa $HOME bb \\$literal ${HOME}xxx $?z zz'",
	" aa\"\\$a\\$b\"xx ", */

	// INPUTS PERSONALIZADOS CON OPERADORES
	//"hola < cara | \"$HOME\" >> caracola | \"$HOME\" << heredoc ",
	//"  cmd1 'literal' > O  \"hola x\" 1234 | cmd2 << H  ",
	//"aa > aa >> bb << bb < cc",

	// REDIRECTION TESTS - Casos complejos/erróneos
	/* "</<</>/>>",                           // Solo operadores (error sintaxis)
	"echo hola >>>>>>> file",              // Múltiples > seguidos (error)
	"echo hola <<<<<<< file",              // Múltiples < seguidos (error)
	"echo hola </<</>/>>   </<</>/>> file", // Operadores mezclados (error)
	"echo hola </<</>/>>   | file",        // Redirección + pipe mezclados (error)
	"echo hola > file </<</>/>>",          // Redirección válida + inválida (error)
	"<< EOF",                              // Heredoc sin comando (error) */

	// ERROR SINTAXIS
	//"'$\"' '$",			// error
	
	NULL
};
    
void test_basic_parser(t_shell *shell)
{
	int index;
    int test_number;
    int passed;
    int failed;

    printf("\n🧪 TESTING PARSER - SIMPLIFIED\n");
    printf("===============================\n");
    
    
    printf("📋 Testing basic parsing pipeline only...\n\n");
    
	index = 0;
    test_number = 0;
    passed = 0;
    failed = 0;

    while (test_cases[index])
    {
        test_number++;
        printf("TEST %d: %s\n", test_number, test_cases[index]);
        
        shell->input = ft_strdup(test_cases[index]);
        
        printf("  1. Syntax analyzer...");
        syntax_analyzer(shell);
        if (!shell->commands_list)
        {
            printf(" ❌ FAILED\n");
            goto cleanup;
        }
        printf(" ✅ OK\n");
        
        printf("  2. Lexical analyzer...");
        lexical_analyzer(shell->commands_list);
        if (!shell->commands_list->words_list)
        {
            printf(" ❌ FAILED\n");
            goto cleanup;
        }
        printf(" ✅ OK\n");
        
        printf("  3. Process commands...");
        process_commands(shell);
        printf(" ✅ OK\n");
        
        printf("  4. Build execution...");
        build_execution_structure(shell->commands_list);
        if (!shell->commands_list->args)
        {
            printf(" ❌ FAILED\n");
            goto cleanup;
        }
        printf(" ✅ OK\n");
        
        // Mostrar resultado
        print_args_array(shell->commands_list->args, "Generated args");
		passed++;
        
cleanup:
        free(shell->input);
        free_commands_list(&shell->commands_list);
        shell->input = NULL;
        shell->commands_list = NULL;
        
        printf("────────────────────────────────\n\n");
        index++;
    }
	
    
    printf("\n📊 BASIC PARSER TEST SUMMARY\n");
    printf("============================\n");
    printf("✅ Passed: \t %d/%d\n", passed, test_number);
    printf("❌ Failed: \t %d/%d\n", failed, test_number);
    printf("Success rate: \t %.1f %%\n", (float) passed / (float) test_number * 100);
    
    if (failed == 0)
        printf("🎉 All basic pipeline tests passed!\n");
	else
		printf("⚠️ %d tests need attention\n", failed);

	cleanup_minishell(shell);		// limpieza    
    exit(0);
}


void test_complex_parser(t_shell *shell)
{
	int index;
    int test_number;
    int passed;
    int failed;

	char **minishell_args;
	char **bash_args;
    
    printf("\n🧪 TESTING PARSER ARGUMENT GENERATION\n");
    printf("=====================================\n");
    printf(" t_cmd->args matches bash argument parsing \n"); 

	printf("\n💡 This test verifies that the parsing pipeline correctly\n");
    printf("   generates the same argument list that bash would receive.\n\n");    
    
    index 		= 0;
	test_number = 0;
	passed 		= 0;
	failed 		= 0;
	
    while (test_cases[index])
    {
		test_number++;
        printf("📋 TEST %d -> \t\t %s\n\n", test_number, test_cases[index]);  		     
        
        shell->input = ft_strdup(test_cases[index]);		// Procesar con minishell
        
        // Ejecutar pipeline completo de minishell
        syntax_analyzer(shell);        
        lexical_analyzer(shell->commands_list);
		process_commands(shell);        
        build_execution_structure(shell->commands_list);
        
        // Obtener argumentos de minishell
        minishell_args = shell->commands_list->args;
        
        // Obtener argumentos esperados (simulando bash)
        bash_args = get_bash_args_real(test_cases[index]);
        
        // Imprimir ambos arrays para comparación visual
        print_args_array(bash_args, "Expected (bash)");
        print_args_array(minishell_args, "Actual (minishell)");
        
        // Comparar arrays
        if (compare_args_arrays(minishell_args, bash_args))
        {
            printf("✅ PASSED -> \t Arguments match\n");
            passed++;
        }
        else
        {
            printf("❌ FAILED -> \t Arguments don't match\n");
            failed++;
        }        
        printf("──────────────────────────────────────\n\n");
        
        // Limpiar memoria
        if (bash_args)
            free_matrix(bash_args);

        free(shell->input);
        free_commands_list(&shell->commands_list);
        shell->input = NULL;
        shell->commands_list = NULL;
        
		index++;
    }        
    cleanup_minishell(shell);		// limpieza
    
    printf("\n📊 PARSER ARGUMENT TEST SUMMARY\n");
    printf("================================\n");
    printf("✅ Passed: \t %d/%d\n", passed, test_number);
    printf("❌ Failed: \t %d/%d\n", failed, test_number);
    printf("Success rate: \t %.1f %%\n", (float) passed / (float) test_number * 100);
    
    if (failed == 0)
        printf("🎉 All argument parsing tests passed!\n");
	else
		printf("⚠️ %d tests need attention\n", failed);

	exit(0);
}

// Función auxiliar para obtener argumentos que bash recibiría
static char **get_bash_args_real(char *input)
{
    int 	pipefd[2];
    pid_t 	pid;
    char 	buffer[4096];
    char 	**args;
    int 	bytes_read;
    char 	*bash_command;
    
    if (!input || !*input)
        return (NULL);
    args = NULL;

    // Crear comando bash que ejecute e imprima argumentos
    bash_command = malloc(strlen(input) + 50);
    sprintf(bash_command, "printf \"%%s\\n\" %s", input);
    
    if (pipe(pipefd) == -1)
    {
        free(bash_command);
        return (NULL);
    }
    
    pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("/bin/bash", "bash", "-c", bash_command, NULL);
        exit(1);
    }
    
    close(pipefd[1]);
    wait(NULL);
    
    bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    free(bash_command);
    
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        if (buffer[bytes_read - 1] == '\n')
            buffer[bytes_read - 1] = '\0';
        args = ft_split(buffer, '\n');
    }
    
    return (args);
}

// Función para comparar arrays de argumentos
static bool compare_args_arrays(char **minishell_args, char **bash_args)
{
    int index;
    
    if (!minishell_args && !bash_args)
        return (true);  
    if (!minishell_args || !bash_args)
		return (false);

	index = 0;    
    while (minishell_args[index] && bash_args[index])
    {
        if (ft_strncmp(minishell_args[index], bash_args[index], ft_strlen(bash_args[index])) != 0)
            return (false);
        index++;
    }
    
    // Verificar que ambos arrays terminan al mismo tiempo
    return (minishell_args[index] == NULL && bash_args[index] == NULL);
}


 // Función para imprimir array de argumentos
static void print_args_array(char **args, const char *label)
{
    int index;
	
	index = 0;
    
    printf("%s ->\t [", label);
    if (args)
    {
        while (args[index])
        {
            printf("\"%s\"", args[index]);
            if (args[index + 1])
                printf(", ");
            index++;
        }
    }
    printf("]\n\n");
}

