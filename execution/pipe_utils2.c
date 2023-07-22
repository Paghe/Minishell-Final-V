/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 13:28:59 by crepou            #+#    #+#             */
/*   Updated: 2023/07/22 17:29:14 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"

void	close_pipes_and_fds(t_cmds **red)
{
	close_pipes(red);
	if ((*red)->data.fd_in != -1)
		close((*red)->data.fd_in);
	if ((*red)->data.fd_out != -1)
		close((*red)->data.fd_out);
}

void	open_files_in_pipes(t_cmds **red)
{
	if ((*red)->data.input)
	{
		(*red)->data.fd_in = open((*red)->data.input, O_RDONLY);
	}
	if ((*red)->data.output)
	{
		if ((*red)->data.is_append)
			(*red)->data.fd_out = open(\
				(*red)->data.output, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			(*red)->data.fd_out = open((*red)->data.output, O_WRONLY);
	}
	redirect_io((*red)->data.fd_in, (*red)->data.fd_out);
}

void	dup_pipes(t_cmds **red, t_cmds **all)
{
	if ((*red)->data.pipe_in != -1)
		dup2((*red)->data.pipe_in, READ_END);
	if ((*red)->data.pipe_out != -1)
		dup2((*red)->data.pipe_out, WRITE_END);
	close_all(all);
}

void	print_string(t_cmds **red, char *str)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strjoin("minishell: ", (*red)->cmds[0]);
	tmp2 = ft_strjoin(tmp, str);
	ft_putendl_fd(tmp2, 2);
	free(tmp);
	free(tmp2);
}

int	exec_full(\
	t_cmds **red, char ***envp, char ***shell_env, t_tokens *tokens)
{
	if (execve((*red)->cmds[0], (*red)->cmds, *envp) == -1)
	{
		if (!access((*red)->cmds[0] + 2, F_OK) \
			&& access((*red)->cmds[0] + 2, X_OK) != 0)
			print_string(red, ": Permission denied");
		else
			print_string(red, ": command not found");
		if (!access((*red)->cmds[0] + 2, F_OK) \
			&& access((*red)->cmds[0] + 2, X_OK) != 0)
		{
			free_everything(red, envp, shell_env, tokens);
			return (126);
		}
		free_everything(red, envp, shell_env, tokens);
		return (-1);
	}
	return (0);
}
