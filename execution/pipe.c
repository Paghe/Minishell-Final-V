/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 21:49:01 by crepou            #+#    #+#             */
/*   Updated: 2023/07/19 13:10:09 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"

int	execution(t_array_cmds cmds, \
	char ***envp, char ***shell_env, t_tokens *tokens)
{
	int	exit_code;

	dup_pipes(&cmds.cmds[cmds.current], cmds.cmds);
	open_files_in_pipes(cmds.cmds);
	if (cmds.cmds[cmds.current]->cmds)
		cmds.cmds[cmds.current]->cmds = escape_quotes_cmds(cmds.cmds[cmds.current]->cmds);
	if (cmds.cmds[cmds.current]->cmds[0] && ft_strncmp(cmds.cmds[cmds.current]->cmds[0], "./", 2) == 0)
	{
		exit_code = execute_full_command(&cmds.cmds[cmds.current], envp, shell_env, tokens);
		if (exit_code != 0)
			return (exit_code);
	}
	else
	{
		if (execve((char const *)cmds.cmds[cmds.current]->data.env, cmds.cmds[cmds.current]->cmds, *envp) == -1)
		{
			print_string(&cmds.cmds[cmds.current], ": command not found");
			free_everything(&cmds.cmds[cmds.current], envp, shell_env, tokens);
			return (-1);
		}
	}
	return (0);
}

int	run_if_builtin(t_cmds **red, char ***envp, char ***shell_env)
{
	int	stdin_terminal;
	int	stdout_terminal;
	int	exit_st;

	stdout_terminal = dup(STDOUT_FILENO);
	stdin_terminal = dup(STDIN_FILENO);
	open_files_in_pipes(red);
	g_exit_c = 0;
	built_in(*red, envp, shell_env, &exit_st);
	redirect_io(stdin_terminal, stdout_terminal);
	close_pipes(red);
	return (exit_st);
}

int	run_if_more_builtins(\
	t_cmds **red, char ***envp, char ***shell_env, t_tokens *tokens)
{
	int	exit_st;

	open_files_in_pipes(red);
	built_in(*red, envp, shell_env, &exit_st);
	free_everything(red, envp, shell_env, tokens);
	exit(exit_st);
}

int	pipe_proccess(t_array_cmds cmds, char ***envp, \
	char ***shell_env, t_tokens *tokens)
{
	int	pid;
	int	exit_code;

	if (cmds.cmds[cmds.current]->data.exist && \
		if_is_builtin(cmds.cmds[cmds.current]->cmds[0]) && count_commands(tokens) == 1)
		return (run_if_builtin(&cmds.cmds[cmds.current], envp, shell_env));
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		if (cmds.cmds[cmds.current]->data.exist && if_is_builtin(cmds.cmds[cmds.current]->cmds[0]))
			exit(run_if_more_builtins(&cmds.cmds[cmds.current], envp, shell_env, tokens));
		else
		{
			exit_code = execution(cmds, envp, shell_env, tokens);
			if (exit_code == -1 || exit_code == 126)
				exit(exit_code);
		}
	}
	close_pipes_and_fds(&cmds.cmds[cmds.current]);
	return (pid);
}
