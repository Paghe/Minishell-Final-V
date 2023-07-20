/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:17:39 by crepou            #+#    #+#             */
/*   Updated: 2023/07/20 01:11:46 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parse.h"
#include "gnl/get_next_line.h"
#include "include/control.h"

char	**copy_env(char **envp)
{
	char	**new_envp;
	int		i;
	int		count;

	i = 0;
	count = count_env_vars(envp);
	new_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			return (new_envp);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	execute_each_cmd(t_tokens *tokens, char ***envp, char ***shell_env)
{
	t_cmds	**cmds;
	int		exit;

	cmds = NULL;
	cmds = init_list_commands(tokens);
	parse_tokens(tokens, cmds, *envp);
	replace_env_vars(&cmds, *shell_env);
	block_signals();
	exit = execute_cmds(cmds, envp, shell_env, tokens);
	free_parse(cmds);
	free(cmds);
	return (exit);
}
