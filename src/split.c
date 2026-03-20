/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asauvage <asauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 13:00:48 by asauvage          #+#    #+#             */
/*   Updated: 2026/03/20 14:28:20 by asauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_w_quote(char *str, int i)
{
	char	quote;

	quote = str[i];
	while (str[i] && str[i] != quote)
		i++;
	return (i);
}

int	count_token(char *str)
{
	int	i;
	int	nb_word;

	i = 0;
	nb_word = 0;
	while (str && str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		nb_word++;
		while (str[i] && !ft_isspace(str[i]))
		{
			if (str[i] == '\'' || str[i] == '\"')
				i = skip_w_quote(str, i);
			i++;
		}
	}
	return (nb_word);
}

int	len_word(char *str, int *end)
{
	int	start;
	int	status;

	status = 0;
	while (ft_isspace(str[*end]))
		*end++;
	start = *end;
	while (str && str[*end] && !ft_isspace(str[*end]))
	{
		if (str[*end] == '\'' || str[*end] == '\"')
		{
			status = 2;
			*end = skip_w_quote(str, *end);
		}
		*end++;
	}
	return (start - *end - status);
}

char	*ft_dup_w(char *res, char *str, int *i)
{
	int	len_w;
	int	start;

	len_w = len_word(str, i);
	res = malloc(sizeof(char) * (len_w + 1));
	if (!res)
	{
		write(2, "hbray: Malloc\n", 15);
		return (NULL);
	}
	start = 0;
	while (start < len_w)
	{
		res[*i - start - 1] = str[*i - start - 1];
		start++;
	}
	res[*i] = '\0';
	return (res);
}

char	**split(char *str)
{
	int		i;
	int		w;
	int		nb_word;
	char	**res;

	nb_word = count_token(str);
	res = malloc(sizeof(char *) * (nb_word + 1));
	if (!res)
		return (NULL);
	i = 0;
	w = 0;
	while (w < nb_word)
	{
		res[w] = ft_dup_w(res[w], str, &i);
		w++;
	}
	res[w] = NULL;
	printf("%d", nb_word);
	return (res);
}
