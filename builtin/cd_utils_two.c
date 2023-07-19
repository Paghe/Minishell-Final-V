/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:47:58 by crepou            #+#    #+#             */
/*   Updated: 2023/07/18 16:49:20 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"
#include "../include/parse.h"

int	cd_env_old(char **env)
{
	int		i;
	char	*dir;

	i = -1;
	dir = NULL;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "OLDPWD=", ft_strlen("OLDPWD=")))
		{
			dir = ft_strdup(env[i] + 7);
			change_old(env);
			if (chdir(dir) != 0)
			{
				perror("OLDPWD not set");
				free(dir);
				return (0);
			}
			change_current_pwd(env);
			free(dir);
			return (1);
		}
	}
	return (0);
}

int	cd_old(char **env, t_cmds *cmds)
{
	int	res;

	res = 0;
	if (!ft_strncmp(cmds[0].cmds[0], "cd", 2) && cmds[0].cmds[1] && \
		!ft_strncmp(cmds[0].cmds[1], "-", 3))
		res = cd_env_old(env);
	return (res);
}
