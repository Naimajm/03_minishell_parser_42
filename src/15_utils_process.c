/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   15_utils_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:13:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/14 13:03:12 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// UTILIDADES PROCESO DESARROLLO

void	print_text_file(const char *filename)
{
	int 	fd; 
	char	*buffer;
	int		bytes_read;
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(ERROR_OPEN_FILE);
		return ;
	}
	buffer = (char *) malloc(sizeof(char) * (1024));
	if (!buffer)
		return ;		
	while ((bytes_read = read(fd, buffer, 1024)) > 0)
		write(1, buffer, bytes_read);
	// liberar memoria
	free(buffer);
	close (fd);
}

void	print_config_shell(t_shell *shell)
{
	if (!shell)
		return ;
	// imprimir copia variables entorno
	//print_strings_array(shell->environment);

	ft_printf("input -> %s\n", shell->input);	
	print_tokens_list(shell->words_list->tokens_list);
}

void	print_strings_array(char **array)
{
	int	index;

	if (!array)
		return ;
	index = 0;
	while (array[index])
	{
		ft_printf("%s\n", array[index]);
		index++;
	}
}