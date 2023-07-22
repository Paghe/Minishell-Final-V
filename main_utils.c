/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:16:01 by crepou            #+#    #+#             */
/*   Updated: 2023/07/22 19:34:42 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parse.h"
#include "gnl/get_next_line.h"
#include "include/control.h"

void	free_after_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		i++;
	}
	if (str)
		free(str);
}

char	**expanded_commands(t_cmds *cmds, int *k)
{
	char	**new_cmds;
	int		j;
	char	**replaced_commands;

	new_cmds = ft_split(cmds->cmds[0], ' ');
	(*k) = 0;
	while (new_cmds && new_cmds[(*k)])
		(*k)++;
	j = 1;
	while (cmds->cmds[j])
		j++;
	replaced_commands = (char **)malloc(sizeof(char *) * ((*k) + j + 1));
	(*k) = 0;
	while (new_cmds[(*k)])
	{
		replaced_commands[(*k)] = ft_strdup(new_cmds[(*k)]);
		(*k)++;
	}
	j = 0;
	while (cmds->cmds[++j])
	{
		replaced_commands[(*k)] = ft_strdup(cmds->cmds[j]);
		(*k)++;
	}
	return (free_after_split(new_cmds), replaced_commands);
}

int	free_the_environ(char **env, int last_pid)
{
	if (*env)
	{
		free(*env);
		*env = NULL;
	}
	if (last_pid == -1 || last_pid == -2)
		return (1);
	return (0);
}

int	replace_cmds(t_cmds **cmds, int i)
{
	int		k;
	char	**replaced_commands;

	k = 0;
	if (cmds[i]->cmds[0][0] == 0)
		return (1);
	if (ft_strncmp(cmds[i]->cmds[0], "/bin/echo ", 10) == 0)
	{
		replaced_commands = expanded_commands(cmds[i], &k);
		replaced_commands[k] = NULL;
		free_after_split(cmds[i]->cmds);
		cmds[i]->cmds = replaced_commands;
	}
	return (0);
}

int	execute_cmds(\
	t_cmds **cmds, char ***envp, char ***shell_env, t_tokens *tokens)
{
	int				i;
	int				last_pid;
	int				prev_pid;
	t_array_cmds	comm;

	init_values(&i, &last_pid, &prev_pid);
	comm.cmds = cmds;
	comm.envp = envp;
	if (no_input(cmds))
		return (0);
	while (cmds[++i])
	{
		check_if_file_exists(cmds[i]);
		if (cmds[i]->cmds[0])
		{
			replace_cmds(cmds, i);
			comm.current = i;
			last_pid = pipe_proccess(comm, envp, shell_env, tokens);
			if (last_pid != -1 && last_pid != -2)
				prev_pid = last_pid;
		}
		if (free_the_environ(&(cmds[i]->data.env), last_pid))
			break ;
	}
	return (wait_process(prev_pid, last_pid));
}
