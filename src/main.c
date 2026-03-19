/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asauvage <asauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:12:33 by asauvage          #+#    #+#             */
/*   Updated: 2026/03/19 17:16:51 by asauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse(char *str)
{
	printf("%s\n", str);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	char	*line;
	char	*pre_line;

	pre_line = NULL;
	while ((line = readline("minishell> ")) != NULL)
	{

		if (ft_strlen(line) > 0 && ft_strcmp(line, pre_line))
			add_history(line);
		if (pre_line)
			free(pre_line);
		parse(line);
		pre_line = ft_strdup(line);
		free(line);
	}
	return (0);
}
