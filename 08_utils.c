/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:13:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/05 13:14:09 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

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