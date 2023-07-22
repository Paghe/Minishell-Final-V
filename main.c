/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apaghera <apaghera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 19:35:49 by apaghera          #+#    #+#             */
/*   Updated: 2023/07/22 14:35:30 by apaghera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parse.h"
#include "gnl/get_next_line.h"
#include "include/control.h"

int	g_exit_c;

int	get_input(char **input)
{
	char	*trimmed;

	signal(SIGINT, cntr_handler);
	signal(SIGQUIT, cntr_handler);
	clear_line();
	if (isatty(STDIN_FILENO))
		(*input) = readline("minishell 🚀 ");
	else
	{
		(*input) = get_next_line(STDIN_FILENO);
		if (!(*input))
			return (1);
		trimmed = ft_strtrim((*input), "\n");
		free((*input));
		if (!trimmed)
			return (1);
		(*input) = trimmed;
	}
	if (!(*input))
		return (free(*input), 1);
	return (0);
}

void	check_grammar(t_tokens *tokens, \
	char ***envp, char ***shell_env, int *exit)
{
	if (!get_grammar(tokens))
		g_exit_c = 2;
	else
		*exit = execute_each_cmd(tokens, envp, shell_env);
}

int	execute(char **envp)
{
	t_lexer	lexer;
	char	*input;
	char	**shell_env;
	int		exit;

	shell_env = copy_env(envp);
	exit = 0;
	while (1)
	{
		if (get_input(&input))
			break ;
		if (input && input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		parsing(&lexer, ft_strdup(input));
		free(input);
		check_grammar(lexer.tokens, &envp, &shell_env, &exit);
		destroy_tokens(lexer.tokens);
		if (exit)
			break ;
	}
	return (free_env(shell_env), free_env(envp), g_exit_c);
}

void	leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	char	**env_vars;

	errno = 0;
	g_exit_c = 0;
	env_vars = copy_env(envp);
	(void)argc;
	(void)argv;
	if ((execute(env_vars)) == -1)
		g_exit_c = -1;
	return (g_exit_c);
}
