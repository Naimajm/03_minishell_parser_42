/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   15_utils_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:13:29 by juagomez          #+#    #+#             */
/*   Updated: 2025/05/14 09:08:14 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

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

char **load_environment_from_file(const char *filename)
{
    int     fd;
    char    *buffer;
    char    *line;
    char    **environment;
    int     count;
    int     bytes_read;
    int     index;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (NULL);

    buffer = (char *) malloc(sizeof(char) * 1024);
    if (!buffer)
        return (NULL);

    environment = (char **)malloc(sizeof(char *) * 1024); // Máximo de 1024 líneas
    if (!environment)
        return (free(buffer), NULL);

    count = 0;
    while ((bytes_read = read(fd, buffer, 1024)) > 0)
    {
        index = 0;
        while (index < bytes_read)
        {
            // Leer línea por línea
            line = (char *)malloc(sizeof(char) * 1024);
            if (!line)
            {
                free(buffer);
                free_environment(environment);
                return (NULL);
            }
			ft_strlcpy(line, buffer, bytes_read);
            //ft_strncpy(line, buffer, bytes_read);
            environment[count++] = line;
        }
    }
    return (environment);
}

void free_environment(char **environment)
{
	int	index;
	
	if (!environment)
        return ;

	index = 0;
	while (environment[index])
	{
		free(environment[index]);
		index++;
	}        
    free(environment);
}

void	print_config_shell(t_shell *shell)
{
	if (!shell)
		return ;
	ft_printf("input -> %s\n", shell->input);
	print_strings_array(shell->environment);
	print_token_list(shell->token_list);

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