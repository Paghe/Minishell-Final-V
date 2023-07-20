/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 19:46:36 by apaghera          #+#    #+#             */
/*   Updated: 2023/07/20 12:52:18 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

char	*replace_var(char *var)
{
	char	*prev;
	char	*curr;
	char	*next;

	prev = var;
	curr = var;
	next = var + 1;
	while (curr && next && *curr && *next)
	{
		if (*prev == '\"' && *curr == '$' && *next == '\"')
			*curr = 17;
		else if (*prev == '\'' && *curr == '$' && *next == '\'')
			*curr = 17;
		prev = curr;
		curr = next;
		next++;
	}
	return (var);
}

char	*put_dollar_back(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == 17)
			str[i] = '$';
		i++;
	}
	return (str);
}

char	*next_dollar(char *str)
{
	int		i;
	int		is_code_var;
	char	*new_var;
	char	*tmp;

	i = -1;
	is_code_var = 0;
	while (str && str[++i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] == '?')
		{
			is_code_var = 1;
			break ;
		}
	}
	new_var = ft_strdup2(str, i);
	tmp = new_var;
	if (is_code_var)
	{
		new_var = ft_strjoin(tmp, ft_itoa(g_exit_c));
		new_var = ft_strjoin(tmp, str + 3);
		free(tmp);
	}
	return (new_var);
}

int	is_assign(char *str)
{
	int	index_first_quote;
	int	index_first_eq;
	int	i;

	index_first_quote = 0;
	index_first_eq = 0;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			index_first_quote = i;
			break ;
		}
		if (str[i] == '=')
		{
			index_first_eq = i;
			break ;
		}
		i++;
	}
	if (index_first_eq > index_first_quote)
		return (1);
	else
		return (0);
}
