/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 01:40:10 by crepou            #+#    #+#             */
/*   Updated: 2023/07/24 01:45:20 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"
#include "../include/control.h"

int	count_dollars(char *word)
{
	int	i;

	i = 0;
	if (!word)
		return (0);
	while (word && *word)
	{
		if (*word == '$')
			i++;
		else
			return (i);
		word++;
	}
	return (i);
}

void	free_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i] != NULL)
	{
		free(envp[i]);
		i++;
	}
	if (envp)
		free(envp);
}

void	init_values(int *i, int *last_pid, int *prev_pid)
{
	*i = -1;
	*last_pid = 0;
	*prev_pid = 0;
}

void	init_export_vals(char **name, char **val, int *tmp)
{
	name = NULL;
	val = NULL;
	*tmp = 0;
}

int	no_input(t_cmds **cmds)
{
	if (cmds[0]->cmds[0])
	{
		if (!cmds[0]->cmds[0][0])
			return (1);
	}
	return (0);
}
