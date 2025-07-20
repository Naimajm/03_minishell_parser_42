/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_testing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:06:59 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/19 11:14:44 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int compare_word_token_structures(t_word *actual, t_word *expected);

void test_lexical_analyzer(t_shell *shell)
{
	int test_number = 0;
	int passed = 0;
	int failed = 0;
	int index = 0;
	
	printf("\n🧪 TESTING LEXICAL ANALYZER\n");
	printf("=================================\n");

	// Lista de casos de prueba
	char *test_cases[] = {
		"echo hello",
		"echo hello > file.txt",
		"echo 'hello world'",
		"ho\"la   \"caracola",
		"cat file.txt | grep pattern",
		"echo    hello     world",
		"echo>file.txt",
		"echo test >> output.txt",
		"echo \"\"",
		"cat << EOF",
		"ls -la < input.txt",
		"echo 'test'\"double\"'end'",
		"\"cat ho\"la' a'$e\" tal'l   ,qu'e\" hac\"e\"s",
		NULL
	};

	// Estructuras esperadas creadas usando add_word_node()
	t_word *expected_results[13];
	
	// Test 0: "echo hello"
	expected_results[0] = NULL;
	add_word_node(&expected_results[0], "echo", WORD);
	add_word_node(&expected_results[0], "hello", WORD);
	
	// Test 1: "echo hello > file.txt"
	expected_results[1] = NULL;
	add_word_node(&expected_results[1], "echo", WORD);
	add_word_node(&expected_results[1], "hello", WORD);
	add_word_node(&expected_results[1], ">", OUTFILE);
	add_word_node(&expected_results[1], "file.txt", WORD);
	
	// Test 2: "echo 'hello world'"
	expected_results[2] = NULL;
	add_word_node(&expected_results[2], "echo", WORD);
	add_word_node(&expected_results[2], "'hello world'", WORD);
	
	// Test 3: "ho\"la   \"caracola"
	expected_results[3] = NULL;
	add_word_node(&expected_results[3], "ho\"la   \"caracola", WORD);
	
	// Test 4: "cat file.txt | grep pattern"
	expected_results[4] = NULL;
	add_word_node(&expected_results[4], "cat", WORD);
	add_word_node(&expected_results[4], "file.txt", WORD);
	add_word_node(&expected_results[4], "|", PIPE);
	add_word_node(&expected_results[4], "grep", WORD);
	add_word_node(&expected_results[4], "pattern", WORD);
	
	// Test 5: "echo    hello     world"
	expected_results[5] = NULL;
	add_word_node(&expected_results[5], "echo", WORD);
	add_word_node(&expected_results[5], "hello", WORD);
	add_word_node(&expected_results[5], "world", WORD);
	
	// Test 6: "echo>file.txt"
	expected_results[6] = NULL;
	add_word_node(&expected_results[6], "echo", WORD);
	add_word_node(&expected_results[6], ">", OUTFILE);
	add_word_node(&expected_results[6], "file.txt", WORD);
	
	// Test 7: "echo test >> output.txt"
	expected_results[7] = NULL;
	add_word_node(&expected_results[7], "echo", WORD);
	add_word_node(&expected_results[7], "test", WORD);
	add_word_node(&expected_results[7], ">>", APPEND);
	add_word_node(&expected_results[7], "output.txt", WORD);
	
	// Test 8: "echo \"\""
	expected_results[8] = NULL;
	add_word_node(&expected_results[8], "echo", WORD);
	add_word_node(&expected_results[8], "\"\"", WORD);
	
	// Test 9: "cat << EOF"
	expected_results[9] = NULL;
	add_word_node(&expected_results[9], "cat", WORD);
	add_word_node(&expected_results[9], "<<", HERE_DOC);
	add_word_node(&expected_results[9], "EOF", WORD);
	
	// Test 10: "ls -la < input.txt"
	expected_results[10] = NULL;
	add_word_node(&expected_results[10], "ls", WORD);
	add_word_node(&expected_results[10], "-la", WORD);
	add_word_node(&expected_results[10], "<", INFILE);
	add_word_node(&expected_results[10], "input.txt", WORD);
	
	// Test 11: "echo 'test'\"double\"'end'"
	expected_results[11] = NULL;
	add_word_node(&expected_results[11], "echo", WORD);
	add_word_node(&expected_results[11], "'test'\"double\"'end'", WORD);

	// Test 12: "\"cat ho\"la' a'$e\" tal'l   ,qu'e\" hac\"e\"s"  // NUEVO CASO
	expected_results[12] = NULL;
	add_word_node(&expected_results[12], "\"cat ho\"la' a'$e\" tal'l   ,qu'e\"", WORD);
	add_word_node(&expected_results[12], "hac\"e\"s", WORD);

	// Ejecución Tests
	while (test_cases[index])
	{
		test_number++;
		printf("\n📋 Test %d: \"%s\"\n", test_number, test_cases[index]);
		
		// Copiar test case a input
		shell->input = ft_strdup(test_cases[index]);
		syntax_analyzer(shell); 
		lexical_analyzer(shell->commands_list);

		printf("Current output:\n");
		print_words_list(shell->commands_list->words_list);

		// Comparar estructuras usando la función de comparación
		int test_passed = compare_word_token_structures(shell->commands_list->words_list, expected_results[index]);
		
		if (test_passed)
		{
			printf("✅ PASSED\n");
			passed++;
		}
		else
		{
			printf("❌ FAILED\n");
			failed++;
		}

		// Limpiar memoria
		free(shell->input);
		free_commands_list(&shell->commands_list);
		shell->commands_list = NULL;

		index++;
	}
	
	// Limpiar estructuras esperadas
	index = 0;
	while (index < 13)
	{
		free_words_list(&expected_results[index]);
		index++;
	}
	
	// Resumen final
	printf("\n📊 SUMMARY\n");
	printf("==================\n");
	printf("✅ Passed: %d/%d\n", passed, test_number);
	printf("❌ Failed: %d/%d\n", failed, test_number);
	printf("Success rate: %.1f%%\n", (float)passed / test_number * 100);
}

// Función de comparación de estructuras t_word_token
static int compare_word_token_structures(t_word *actual, t_word *expected)
{
	t_word *current_actual = actual;
	t_word *current_expected = expected;
	
	// Comparar nodo por nodo
	while (current_actual && current_expected)
	{
		// Comparar word_type
		if (current_actual->word_type != current_expected->word_type)
		{
			printf("   ERROR: word_type mismatch - Expected '%c', got '%c'\n", 
				   current_expected->word_type, current_actual->word_type);
			return (0);
		}
		
		// Comparar raw_word
		if (!current_actual->raw_word || !current_expected->raw_word)
		{
			if (current_actual->raw_word != current_expected->raw_word)
			{
				printf("   ERROR: raw_word NULL mismatch\n");
				return (0);
			}
		}
		else if (strcmp(current_actual->raw_word, current_expected->raw_word) != 0)
		{
			printf("   ERROR: raw_word mismatch - Expected '%s', got '%s'\n", 
				   current_expected->raw_word, current_actual->raw_word);
			return (0);
		}
		
		// Comparar processed_word (puede ser NULL en lexical_analyzer)
		if (current_actual->processed_word != current_expected->processed_word)
		{
			if (current_actual->processed_word && current_expected->processed_word)
			{
				if (strcmp(current_actual->processed_word, current_expected->processed_word) != 0)
				{
					printf("   WARNING: processed_word mismatch - Expected '%s', got '%s'\n", 
						   current_expected->processed_word, current_actual->processed_word);
				}
			}
		}
		
		// Avanzar al siguiente nodo
		current_actual = current_actual->next;
		current_expected = current_expected->next;
	}
	
	// Verificar que ambas listas terminaron al mismo tiempo
	if (current_actual || current_expected)
	{
		printf("   ERROR: List length mismatch\n");
		if (current_actual)
			printf("   Actual list has extra tokens\n");
		if (current_expected)
			printf("   Expected list has extra tokens\n");
		return (0);
	}
	
	return (1); // TRUE - estructuras son iguales
}

