/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_replacement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 23:22:34 by crepou            #+#    #+#             */
/*   Updated: 2023/07/24 03:31:30 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

int	count_all_dollars(char *word)
{
	int	i;
	int	count;
	int	flag_type_q;
	int	flag_type_v;

	i = 0;
	count = 0;
	flag_type_q = 0;
	flag_type_v = 0;
	while (word[i])
	{
		if (word[i] == '$')
			count++;
		if (word[i - 1] == '$' && word[i] && word[i] != '\"')
		{
			flag_type_q = 1;
			break ;
		}
		if (word[i - 1] == '$' && word[i] && word[i] == '\"')
			flag_type_v = 1;
		i++;
	}
	if (flag_type_q && flag_type_v)
		return (count - 1);
	return (count);
}

char	*create_dollar_str(char *final, char *cmds, int dollars, char *new_val)
{
	char	*dollar_str;
	char	*tmp;
	char	*tmp2;
	char	*tmp_final;

	tmp = NULL;
	dollar_str = ft_strdup2(cmds, dollars - 1);
	tmp2 = escape_dquote(cmds);
	if (count_all_dollars(cmds) == dollars && \
		ft_strncmp(new_val, " ", 2) != -32)
		tmp = ft_strjoin(dollar_str, new_val);
	else if (count_all_dollars(cmds) == dollars)
		tmp = ft_strjoin(dollar_str, new_val);
	else if (ft_strlen(tmp2) > 1)
		tmp = ft_strjoin(dollar_str, tmp2 + 1);
	free(tmp2);
	tmp_final = ft_strjoin(final, tmp);
	if (tmp)
		free(tmp);
	free(dollar_str);
	if (final)
		free(final);
	return (tmp_final);
}

char	*replace_not_env_var(char *variable, char **final)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	tmp2 = NULL;
	tmp = variable;
	variable = remove_char_from_word(variable, '\"');
	tmp = put_dollar_back(variable);
	variable = next_dollar(tmp);
	free(tmp);
	tmp2 = *final;
	*final = ft_strjoin(*final, variable);
	free(variable);
	free(tmp2);
	return (*final);
}

char	*replace_command(char *cmd, char *arg2, char **envp, int sum)
{
	int		dollars;
	char	*variable;
	char	*new_val;
	char	*final;
	int		k;

	final = NULL;
	new_val = NULL;
	while (sum > -1)
	{
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
			final = replace_not_env_var(variable, &final);
		free_vars(&variable, &new_val);
	}
	return (final);
}

int	has_a_dollar(char *str)
{
	int	i;
	int	flag_dollar;

	i = 0;
	flag_dollar = 0;
	while (str[i])
	{
		if (str[i] != '\"' && str[i] != '\'' && flag_dollar)
			return (1);
		if (str[i] == '$' && str[i + 1] && str[i + 1] != '\"')
			flag_dollar = 1;
		if (str[i] == '$' && str[i + 1] && str[i + 1] == '\"')
			break ;
		i++;
	}
	return (0);
}
