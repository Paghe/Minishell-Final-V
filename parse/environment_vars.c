/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_vars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 13:59:13 by crepou            #+#    #+#             */
/*   Updated: 2023/07/24 00:26:45 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"
#include "../include/control.h"

char	*ft_strdup2(const char *s1, int stop)
{
	char	*s2;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(s1);
	if (!s1)
		return (0);
	s2 = malloc(sizeof (char) * len + 1);
	if (!s2)
		return (NULL);
	while (s1[i] != '\0' && i < stop)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

char	**create_final_str(char *str, char **final, int *count, int *end)
{
	char	**after;
	int		i;

	if (!str)
		return (NULL);
	if (!ft_strncmp(str, "=", 2))
		return (NULL);
	*final = NULL;
	if (*str && *str == ' ')
		*final = " ";
	after = ft_split(str, ' ');
	i = 0;
	while (str[i])
		i++;
	*end = 0;
	if ((i > 0) && str[i - 1] == ' ')
		*end = 1;
	*count = 0;
	while (after[*count])
		(*count)++;
	return (after);
}

char	*remove_spaces(char *str, int *identify, int end_with_space, int i)
{
	char	**after;
	char	*tmp;
	char	*final;
	int		count;

	after = create_final_str(str, &final, &count, &end_with_space);
	tmp = NULL;
	while (after[++i])
	{
		final = ft_strjoin(final, after[i]);
		if (tmp)
			free(tmp);
		tmp = final;
		if (i < count - 1 || end_with_space)
		{
			final = ft_strjoin(final, " ");
			if (tmp)
				free(tmp);
			tmp = final;
		}
	}
	if (i == 0)
		return (*identify = 1, free_after_split(after), str);
	return (free_after_split(after), final);
}

int	check_after_trim_quotes(char *word, int *identify)
{
	char	*tmp_trim;

	tmp_trim = ft_strtrim(word, "\'\"");
	*identify = 0;
	if (*word == '?' || *(tmp_trim) == '=')
	{
		free(tmp_trim);
		return (0);
	}
	free(tmp_trim);
	return (1);
}

int	is_env_var(char *word, char	**var_name, char **value, char **env)
{
	char	*after_eq;
	int		index_bef_eq;
	int		identify;
	char	*tmp;

	if (!check_after_trim_quotes(word, &identify))
		return (0);
	after_eq = ft_strchr(word, '=');
	if (after_eq)
	{
		index_bef_eq = after_eq - word;
		(*value) = ft_strtrim((after_eq + 1), "\'\"");
		tmp = (*value);
		(*value) = remove_spaces((*value), &identify, 0, -1);
		if (!identify)
			free(tmp);
		tmp = (*value);
		if ((*value) && (*value)[0] == '$')
		{
			(*value) = ft_strdup(get_env_var((*value) + 1, env));
			free(tmp);
		}
		return ((*var_name) = ft_strdup2(word, index_bef_eq), 1);
	}
	return (-1);
}
