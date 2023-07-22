/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:44:30 by crepou            #+#    #+#             */
/*   Updated: 2023/07/22 19:28:02 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"
#include "../include/control.h"

void	error_message_export(char *tmp, char **cmds)
{
	tmp = ft_strtrim(cmds[1], "\'\"");
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(tmp, 2);
	free(tmp);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_exit_c = 1;
}

void	export(char **cmds, char ***env, char ***shell_env)
{
	char	*var_name;
	char	*value;
	int		is_var;
	char	*tmp;

	init_export_vals(&var_name, &value, &is_var);
	tmp = NULL;
	if (cmds && cmds[1])
		is_var = is_env_var(cmds[1], &var_name, &value, *shell_env);
	if (is_var == 1)
	{
		set_env_var(env, var_name, value);
		set_env_var(shell_env, var_name, value);
		free(var_name);
		free(value);
	}
	else if (is_var == -1)
		return ;
	else
	{
		g_exit_c = 1;
		if (cmds && cmds[1])
			error_message_export(tmp, cmds);
	}
}
