/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10.3_utils_debug.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:13:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/07/31 14:10:46 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_text_file(const char *filename)
{
	int 	fd; 
	char	*buffer;
	int		bytes_read;
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putendl_fd(ERROR_OPEN_FILE, STDERR_FILENO);
		return ;
	}
	buffer = (char *) malloc(sizeof(char) * (1024));
	if (!buffer)
		return ;		
	while ((bytes_read = read(fd, buffer, 1024)) > 0)
		write(1, buffer, bytes_read);
	free(buffer);
	close (fd);
}

void	print_config_shell(t_shell *shell)
{
	if (!shell)
		return ;
	
	//print_strings_array(shell->environment); // imprimir copia variables entorno
	printf("input -> %s\n", shell->input);	
	print_commands_list(shell->commands_list);	
}

void	print_strings_array(char **array)
{
	int	index;

	if (!array)
		return ;
	index = 0;
	while (array[index])
	{
		printf("%s\n", array[index]);
		index++;
	}
}