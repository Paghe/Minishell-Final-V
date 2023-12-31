/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:44:30 by crepou            #+#    #+#             */
/*   Updated: 2023/07/19 13:10:09 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.h"
#include "../include/control.h"

void	export(char **cmds, char ***env, char ***shell_env)
{
	char	*var_name;
	char	*value;
	int		is_var;

	var_name = NULL;
	value = NULL;
	is_var = 0;
	if (cmds && cmds[1])
	{
		//printf("command: %s\n", cmds[1]);
		is_var = is_env_var(cmds[1], &var_name, &value, *shell_env);
	}
	if (is_var == 1)
	{
		set_env_var(env, var_name, value);
		set_env_var(shell_env, var_name, value);
		free(var_name);
		free(value);
	}
	else if (is_var == -1)
	{
		 
		return ;
	}
	else
	{
		g_exit_c = 1;
		char *tmp;
		char *tmp2;
		free(var_name);
		free(value);
		if (cmds && cmds[1])
		{
			tmp2 = ft_strtrim(cmds[1], "\'\"");
			tmp = ft_strjoin("minishell: export: `", tmp2);
			free(tmp2);
			tmp2 = ft_strjoin(tmp, "': not a valid identifier");
			ft_putendl_fd(tmp2, 2);
			free(tmp);
			free(tmp2);
		}
	}
}
