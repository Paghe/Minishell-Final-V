/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_replacement_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 03:10:09 by crepou            #+#    #+#             */
/*   Updated: 2023/07/24 16:27:28 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

char	*escape_dquote(char	*cmds)
{
	int		i;
	int		j;
	char	*new_cmds;

	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (cmds[i] != '\"')
			j++;
		i++;
	}
	new_cmds = malloc(sizeof(char) * (j + 1));
	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (cmds[i] && (cmds[i] != '\"'))
			new_cmds[j++] = cmds[i];
		i++;
	}
	new_cmds[j] = '\0';
	return (new_cmds);
}

void	replace_environ_var(t_cmds **cmds, char **envp, int i, int j)
{
	char	*arg2;
	char	*final;
	char	*tmp;

	final = NULL;
	arg2 = ft_strdup(cmds[i]->cmds[j]);
	if (j > 0 || cmds[i]->cmds[j][0] == '$')
	{
		if (has_a_dollar(cmds[i]->cmds[j]))
		{
			tmp = escape_dquote(cmds[i]->cmds[j]);
			final = replace_command(\
				tmp, tmp, envp, ft_strlen(tmp));
			free(tmp);
		}
		else
			final = replace_command(\
				cmds[i]->cmds[j], arg2, envp, ft_strlen(arg2));
		free(cmds[i]->cmds[j]);
		cmds[i]->cmds[j] = final;
	}
	if (arg2)
		free(arg2);
}

void	replace_env_vars(t_cmds ***cmds, char **envp)
{
	int		i;
	int		j;

	i = -1;
	while ((*cmds)[++i])
	{
		j = 0;
		while ((*cmds)[i]->cmds[j] && (*cmds)[i]->data.exist)
		{
			if ((ft_strnstr((*cmds)[i]->cmds[j], "-n", ft_strlen(\
				(*cmds)[i]->cmds[j]))) || is_assign((*cmds)[i]->cmds[j]))
			{
				j++;
				continue ;
			}
			replace_environ_var(*cmds, envp, i, j);
			j++;
		}
	}
}

void	free_vars(char **variable, char **new_val)
{
	if (*variable)
		free(*variable);
	if (*new_val)
		free(*new_val);
}
