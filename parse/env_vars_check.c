/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apaghera <apaghera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 13:12:06 by crepou            #+#    #+#             */
/*   Updated: 2023/07/19 14:43:58 by apaghera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

int	count_char_in_word(char *str, char c)
{
	int	i;

	i = 0;
	while (str && *str)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

char	*remove_char_from_word(char *str, char c)
{
	int		count;
	int		i;
	char	*new_str;

	count = ft_strlen(str) - count_char_in_word(str, c);
	if ((size_t) count == ft_strlen(str))
	{
		count = ft_strlen(str) - count_char_in_word(str, '\'');
		c = '\'';
	}
	new_str = (char *)malloc(count + 1);
	i = 0;
	while (i < count)
	{
		if (*str == c)
		{
			str++;
			continue ;
		}
		new_str[i] = *str;
		i++;
		str++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	check_quote_type(char **real_str, int *i)
{
	int	quote_type;

	quote_type = 17;
	if ((*real_str) && *(*real_str) && *(*real_str) == '$')
	{
		((*real_str))++;
		(*i)++;
	}
	if ((*real_str) && *(*real_str) && (*(*real_str)) == '\"')
	{
		quote_type = '\"';
		((*real_str))++;
		(*i)++;
	}
	else if ((*real_str) && *(*real_str) && (*(*real_str)) == '\'')
	{
		quote_type = '\'';
		((*real_str))++;
		(*i)++;
	}
	return (quote_type);
}

void	stop_if_quote_type(char	**real_str, int quote_type, int *i)
{
	while ((*real_str) && *(*real_str) && *(*real_str) != quote_type)
	{
		((*real_str))++;
		(*i)++;
	}
}

void	stop_if_any_quote(char	**real_str, int *i)
{
	while ((*real_str) && *(*real_str))
	{
		if (*(*real_str) == '\"' || *(*real_str) == '\'' || *(*real_str) == '$')
		{
			(*i)--;
			((*real_str))--;
			break ;
		}
		((*real_str))++;
		(*i)++;
	}
}
