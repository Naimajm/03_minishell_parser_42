/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcorona <emcorona@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:48:45 by emcorona          #+#    #+#             */
/*   Updated: 2025/08/05 20:25:29 by emcorona         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../inc/minishell.h"

void	ft_exit_error(char *error)
{
	printf("%s\n", error);
	exit (EXIT_FAILURE);
}

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		ft_exit_error("minishell: Memory allocation error");
	return (ret);
}

char	**ft_copy_env(char **env)
{
	int		i;
	int		j;
	char	**cpy;

	i = 0;
	while (env[i])
		i++;
	cpy = safe_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		cpy[i] = safe_malloc(sizeof (char) * (ft_strlen(env[i]) + 1));
		j = 0;
		while (env[i][j])
		{
			cpy[i][j] = env[i][j];
			j++;
		}
		cpy[i][j] = '\0';
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

char	*ft_substr_malloc(const char *input, int start, int len)
{
	char	*substr;

	if (!input || start < 0 || len < 0)
		return (NULL);
	substr = safe_malloc(sizeof(char) * (len + 1)); // buscar donde está 
	ft_strlcpy(substr, &input[start], len);
	return (substr);
}

void ft_shellevel(t_shell *shell)
{
	char	*str_value;
	char	*str_value_shlvl;
	int		int_value;

	//buscar variable de entorno SHLVL y obtengo el valor
	str_value = get_environment_var(shell->environment, "SHLVL");
	//pasarlo a interger e incrementarlo
	int_value = ft_atoi(str_value); // TODO CON MENOS 1???
	int_value++;
	//Liberamos el espacio en memoria creado por get_environment y convertir int a str
	free(str_value);
	str_value = ft_itoa(int_value);
	//Componer de nuevo el valor SHLVL=int
	str_value_shlvl = ft_strjoin("SHLVL=", str_value);
	//actualizar el contenido de la variable de entorno.
	shell->environment = ft_add_modify_env(shell->environment, str_value_shlvl, ft_valid_env_var(str_value_shlvl));
	free(str_value);
	free(str_value_shlvl);
}


// TODO: MI VERSION DE SHELLEVEL INDEPENDIZANDO DE SHELBASH EL CONTADOR. creo que coge el uno de la bashell o zshell, pero yo creo que al ejectuar por primera vez nuestra minishell esta deberia mostrar 1 nivel, sin contar el de bashell O ZSHELL
/* void ft_shellevel(t_shell *shell)
{
	char	*str_value;
	char	*str_value_shlvl;
	int		int_value;
	int		flag_level;

	flag_level = 1;
	str_value = get_environment_var(shell->environment, "SHLVL");
	if (flag_level > 1) //buscar variable de entorno SHLVL y obtengo el valor
	{
		str_value = get_environment_var(shell->environment, "SHLVL");
		//pasarlo a interger e incrementarlo
		int_value = ft_atoi(str_value);
		int_value++;
		// valor como entero lo pasamos a string
		str_value = ft_itoa(int_value);
		//Liberamos el espacio en memoria creado por get_environment y convertir int a str
		free(str_value);
	}
	else // primera iteración con shell level 1
	{
		// valor como entero lo pasamos a string
		str_value = ft_itoa(1);
		flag_level++;
	}
	//Componer de nuevo el valor SHLVL=int
	str_value_shlvl = ft_strjoin("SHLVL=", str_value);
	//actualizar el contenido de la variable de entorno.
	shell->environment = ft_add_modify_env(shell->environment, str_value_shlvl, ft_valid_env_var(str_value_shlvl));
	free(str_value);
	free(str_value_shlvl);
} */
