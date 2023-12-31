/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 19:59:22 by apaghera          #+#    #+#             */
/*   Updated: 2023/07/18 14:57:36 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/control.h"
#include "../include/parse.h"

char	*escape_quote(char	*cmds)
{
	int		i;
	int		j;
	char	*new_cmds;

	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (cmds[i] != '\"' && cmds[i] != '\'')
			j++;
		i++;
	}
	new_cmds = malloc(sizeof(char) * (j + 1));
	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (cmds[i] && (cmds[i] != '\"' && cmds[i] != '\''))
		{
			new_cmds[j] = cmds[i];
			j++;
		}
		i++;
	}
	new_cmds[j] = '\0';
	return (new_cmds);
}

void	no_quote(t_cmds *cmds)
{
	int		i;
	char	*tmp;

	i = 0;
	while (cmds->cmds[i])
	{
		tmp = escape_quote(cmds->cmds[i]);
		if (cmds->cmds[i])
			free(cmds->cmds[i]);
		cmds->cmds[i] = ft_strdup(tmp);
		free(tmp);
		i++;
	}
}

int	count_commands(t_tokens	*tokens)
{
	t_token	*current;
	int		count;

	count = 1;
	current = tokens->front;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

void	parse_tokens(t_tokens *tokens, t_cmds **cmds, char **envp)
{
	t_token	*current;
	int		i;
	int		j;
	int		fd;
	int		flag;

	current = tokens->front;
	i = 0;
	j = 0;
	flag = 1;
	while (current)
	{
		if (current->type == PIPE)
		{
			flag = 0;
			init_pipes(cmds, i);
			i++;
			current = current->next;
			j = 0;
		}
		if (is_input_redirect(current))
		{
			if (current->type == DLESS)
			{
				current = current->next;
				if (cmds[i]->data.input)
					free(cmds[i]->data.input);
				cmds[i]->data.input = ft_strdup("temp_file");
				here_doc(current, cmds[i]);
				if (current->next && is_the_word(current->next))
					current = current->next;
				else
					break ;
			}
			if ((current->type == LESS) && (current->next && is_the_word(current->next)))
			{
				if (cmds[i]->data.input)
					free(cmds[i]->data.input);
				cmds[i]->data.input = ft_strdup(current->next->token);
				if (current->next && is_the_word(current->next))
					current = current->next;
			}
			if (flag == 1)
				cmds[i]->data.is_redir_first = 1;
		}
		if (is_output_redirect(current))
		{
			if (flag == 1)
				cmds[i]->data.is_redir_first = 1;
			if (current->type == DMORE)
				cmds[i]->data.is_append = 1;
			if (current->next && is_the_word(current->next))
			{
				if (cmds[i]->data.output)
					free(cmds[i]->data.output);
				cmds[i]->data.output = ft_strdup(current->next->token);
				if (cmds[i]->data.is_append)
					fd = open(current->next->token, O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd = open(current->next->token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				close(fd);
				current = current->next;
				if (current->next && is_the_word(current->next))
					cmds[i]->data.exist = 1;
			}
		}
		else
		{
			flag = 0;
			if (cmds[i]->data.env)
				free(cmds[i]->data.env);
			cmds[i]->cmds[j] = ft_strdup(current->token);
			cmds[i]->data.exist = 1;
			char *tmp = ft_strtrim(cmds[i]->cmds[0], "\"");
			if (cmds[i]->cmds[0])
				free(cmds[i]->cmds[0]);
			cmds[i]->cmds[0] = ft_strdup(tmp);
			free(tmp);
			cmds[i]->data.env = get_env_path(envp, cmds[i]->cmds[0]);
			j++;
			cmds[i]->cmds[j] = NULL;
		}
		if (current)
		{
			flag = 0;
			current = current->next;
		}
	}
}

void	free_parse(t_cmds **cmds)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i]->cmds && cmds[i]->cmds[j])
		{
			if (cmds[i]->data.exist)
				free(cmds[i]->cmds[j]);
			j++;
		}
		if (cmds[i]->cmds)
			free(cmds[i]->cmds);
		if (cmds[i]->data.env)
			free(cmds[i]->data.env);
		if (cmds[i]->data.input)
			free(cmds[i]->data.input);
		if (cmds[i]->data.output)
			free(cmds[i]->data.output);
		free(cmds[i]);
		i++;
	}
}
