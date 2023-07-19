/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_replacement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 23:22:34 by crepou            #+#    #+#             */
/*   Updated: 2023/07/18 23:23:56 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

char	*create_dollar_str(char *final, char *cmds, int dollars, char *new_val)
{
	char	*dollar_str;
	char	*tmp;

	dollar_str = ft_strdup2(cmds, dollars - 1);
	tmp = ft_strjoin(dollar_str, new_val);
	final = ft_strjoin(final, tmp);
	free(dollar_str);
	return (final);
}

char	*replace_not_env_var(char *variable, char *final)
{
	char	*tmp;

	variable = remove_char_from_word(variable, '\"');
	tmp = put_dollar_back(variable);
	variable = next_dollar(tmp);
	final = ft_strjoin(final, variable);
	return (final);
}

char	*replace_command(char *cmd, char *arg2, char **envp, int sum)
{
	int		dollars;
	char	*variable;
	char	*new_val;
	char	*final;
	int		k;

	while (sum > -1)
	{
		k = 0;
		variable = next_var(arg2, arg2, &k);
		dollars = count_dollars(arg2);
		if (k == -1)
			break ;
		sum -= k;
		arg2 = arg2 + k;
		new_val = get_next_var(variable, envp);
		if (dollars % 2 != 0 && new_val != NULL)
			final = create_dollar_str(final, cmd, dollars, new_val);
		if (!new_val)
			final = replace_not_env_var(variable, final);
	}
	if (new_val)
		free(new_val);
	return (final);
}

void	replace_environ_var(t_cmds **cmds, char **envp, int i, int j)
{
	char	*arg2;
	char	*final;

	final = NULL;
	arg2 = ft_strdup(cmds[i]->cmds[j]);
	if (j > 0 || cmds[i]->cmds[j][0] == '$')
	{
		if (*arg2 == '?')
			final = replace_command(\
				cmds[i]->cmds[j], arg2 + 1, envp, ft_strlen(arg2));
		else
			final = replace_command(\
				cmds[i]->cmds[j], arg2, envp, ft_strlen(arg2));
		free(cmds[i]->cmds[j]);
		cmds[i]->cmds[j] = final;
	}
	if (arg2)
		free(arg2);
}

void	replace_env_vars(t_cmds **cmds, char **envp)
{
	int		i;
	int		j;

	i = -1;
	while (cmds[++i])
	{
		j = 0;
		while (cmds[i]->cmds[j] && cmds[i]->data.exist)
		{
			if ((ft_strnstr(cmds[i]->cmds[j], "-n", ft_strlen(\
				cmds[i]->cmds[j]))) || is_assign(cmds[i]->cmds[j]))
			{
				j++;
				continue ;
			}
			replace_environ_var(cmds, envp, i, j);
			j++;
		}
	}
}
