/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_check_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 02:48:44 by crepou            #+#    #+#             */
/*   Updated: 2023/07/24 16:18:43 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

char	*next_var(char *str, char	*real_str, int *i)
{
	char	quote_type;
	char	*new_var;

	*i = 0;
	if (real_str && *real_str && (*real_str \
		== '\"' || *real_str == '\'' || *real_str == '$'))
	{
		quote_type = check_quote_type(&real_str, i);
		if (quote_type != 17)
			stop_if_quote_type(&real_str, quote_type, i);
		else
			stop_if_any_quote(&real_str, i);
		(*i)++;
	}
	else
	{
		stop_if_any_quote(&real_str, i);
		(*i)++;
	}
	new_var = ft_strdup2(str, (real_str) - str + 1);
	return (new_var);
}

void	replace_with_exit_c(char **final)
{
	char	*num_tmp;
	char	*tmp2;

	num_tmp = ft_itoa(g_exit_c);
	tmp2 = ft_strjoin(*final, num_tmp);
	free(*final);
	*final = ft_strdup(tmp2);
	free(tmp2);
	free(num_tmp);
}

void	replace_if_not_quote(char **final, char *start, char **envp)
{
	char	*tmp2;

	tmp2 = ft_strjoin(*final, get_env_var(start, envp));
	free(*final);
	*final = ft_strdup(tmp2);
	free(tmp2);
}

void	replace_if_quote(char **final, char *start)
{
	char	*tmp2;
	char	*after_trim;

	after_trim = remove_char_from_word(start, '\"');
	tmp2 = ft_strjoin(*final, after_trim);
	free(*final);
	*final = ft_strdup(tmp2);
	free(tmp2);
	*final = put_dollar_back(*final);
	free(after_trim);
}

char	*get_next_var(char *var, char **envp)
{
	char	**start;
	char	*final;
	int		i;

	if (!var)
		return (NULL);
	if (*var != '$')
		return (NULL);
	start = ft_split(var, '$');
	i = 0;
	final = NULL;
	while (start[i])
	{
		if (start[i][0] == '?')
			replace_with_exit_c(&final);
		else if (start[i][0] != '\"' && start[i][0] != '\'')
			replace_if_not_quote(&final, start[i], envp);
		else
			replace_if_quote(&final, start[i]);
		free(start[i]);
		i++;
	}
	free(start);
	return (final);
}
