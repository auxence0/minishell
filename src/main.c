/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asauvage <asauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:12:33 by asauvage          #+#    #+#             */
/*   Updated: 2026/03/19 22:27:52 by asauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	find_substr(char *find, char *str)
{
	int	i;
	int	j;
	int	len;
	int	len_str;

	i = 0;
	len_str = ft_strlen(str);
	len = ft_strlen(find);
	while (str && str[i] && i < len_str - len)
	{
		j = 0;
		while (j < len && find[j++] == str[i++]);
		if (j == len)
			return (1);
		i = i - j + 1;
	}
	return (0);
}

char	*find_pwd(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (find_substr("PWD=", str[i]))
			return (&str[i][4]);
		i++;
	}
	return (NULL);
}

void	cd(t_token *token)
{
	char	*path;
	int		fd;
	path = find_pwd(token->env);
	fd = chdir(ft_strjoin(path, token->token[1]));
	if (fd == -1)
	{
		perror("hbray:");
		exit (1);
	}
}

void	parse(char *str, t_token *token)
{
	token->token = ft_split(str, ' ');
	if (ft_strcmp(token->token[0], "cd"))
		cd(token);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	char	*pre_line;
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		write(2, "Error: Malloc\n", 15);
		return (1);
	}
	token->env = envp;
	pre_line = NULL;
	while ((line = readline("minishell> ")) != NULL)
	{

		if (ft_strlen(line) > 0)
			add_history(line);
		if (pre_line)
			free(pre_line);
		parse(line, token);
		pre_line = ft_strdup(line);
		free(line);
	}
	return (0);
}
