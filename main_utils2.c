/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apaghera <apaghera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:17:39 by crepou            #+#    #+#             */
/*   Updated: 2023/07/21 19:41:59 by apaghera         ###   ########.fr       */
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

void	free_list_cmds(t_cmds **red, char ***envp, char ***shell_env)
{
	free_env(*envp);
	free_env(*shell_env);
	free_parse(red);
}

void	check_if_file_exists(t_cmds *cmds)
{
	int	fd;

	if (cmds->data.input)
	{
		fd = open(cmds->data.input, O_RDONLY);
		if (fd < 0)
			perror(cmds->data.input);
		close(fd);
	}
}

void	free_val(char *new_val)
{
	if (new_val)
		free(new_val);
}
