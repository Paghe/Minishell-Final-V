/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 12:22:24 by crepou            #+#    #+#             */
/*   Updated: 2023/07/21 17:03:24 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"

int	init_pipes(t_cmds **cmds, int index)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	cmds[index]->data.pipe_out = pipe_fd[WRITE_END];
	if (cmds[index + 1])
		cmds[index + 1]->data.pipe_in = pipe_fd[READ_END];
	return (0);
}

void	close_all(t_cmds **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (cmds[i]->data.pipe_in != -1)
			close(cmds[i]->data.pipe_in);
		if (cmds[i]->data.pipe_out != -1)
			close(cmds[i]->data.pipe_out);
		i++;
	}
}

void	free_everything(
		t_cmds **red, char ***envp, char ***shell_env, t_tokens *tokens)
{
	free_env(*envp);
	free_env(*shell_env);
	free_parse(red);
	free(red);
	destroy_tokens(tokens);
}

int	wait_process(int last_pid, int last)
{
	int	exit;
	int	status;
	int	e_code;

	exit = 0;
	e_code = 0;
	while (exit != -1)
	{
		exit = waitpid(-1, &status, 0);
		if (exit == last_pid)
			e_code = WEXITSTATUS(status);
	}
	if (last != -2)
	{
		if (g_exit_c != 1 && g_exit_c != 126)
			g_exit_c = e_code;
		if (e_code == 255)
			g_exit_c = 127;
	}
	if (last == -1 || last == -2)
		return (1);
	else
		return (0);
}

void	close_pipes(t_cmds **red)
{
	if ((*red)->data.pipe_in != -1)
		close((*red)->data.pipe_in);
	if ((*red)->data.pipe_out != -1)
		close((*red)->data.pipe_out);
}
