/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crepou <crepou@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:17:49 by apaghera          #+#    #+#             */
/*   Updated: 2023/07/24 03:30:29 by crepou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "lexer.h"
# include <sys/types.h>
# include <sys/wait.h>
# include "../gnl/get_next_line.h"

# define READ_END 0
# define WRITE_END 1

extern int	g_exit_c;

typedef struct s_data
{
	char		*env;
	char		*input;
	char		*output;
	int			fd_in;
	int			fd_out;
	int			pipe_in;
	int			pipe_out;
	int			is_append;
	int			is_redir_first;
	int			is_updated;
	int			out_redir_first;
	int			exist;
	int			index;
}	t_data;

typedef struct s_cmds
{
	char	**cmds;
	t_data	data;
}	t_cmds;

typedef struct s_array_cmds
{
	t_cmds	**cmds;
	int		current;
	char	***envp;
}	t_array_cmds;

char	*escape_quote(char	*cmds);
void	no_quote(t_cmds *cmds);
int		is_symbol(t_token *token);
int		check_first_token(t_token	*token);
int		not_correct_pos(t_token *current);
int		get_grammar(t_tokens *tokens);
int		is_word(t_token *token);
void	parse_tokens(t_tokens *tokens, t_cmds **cmds, char **envp);
void	create_commands(t_cmds **cmds, t_token *current, int i, int j);
t_token	*handle_input(t_token *current, t_cmds **cmds, int i);
t_token	*handle_output(t_token *current, t_cmds **cmds, int i);
t_token	*handle_pipes(t_token *current, t_cmds **cmds, int *i, int *j);
int		is_input_redirect(t_token *token);
int		is_output_redirect(t_token *token);
int		is_the_word(t_token *token);
t_cmds	**init_list_commands(t_tokens *tokens);
int		count_commands(t_tokens *tokens);
void	free_parse(t_cmds **cmds);
void	input_redirection(t_cmds **red, char **envp);
void	output_redirection_renew(t_cmds **red, char **envp);
void	redirect_io(int input, int output);
int		begin_with_pipes(t_token *token);
int		init_pipes(t_cmds **cmds, int index);
void	close_all(t_cmds **cmds);
int		pipe_proccess(t_array_cmds cmds, char ***envp, \
			char ***shell_env, t_tokens *tokens);
int		is_env_var(char *word, char	**var_name, char **value, char **env);
char	*get_env_var(char *var_name, char **envp);
void	replace_env_vars(t_cmds ***cmds, char **envp);
int		count_dollars(char *word);
char	**escape_quotes_cmds(char **cmds);
int		unset(char ***envp, char *var_name);
void	print_env(char **envp);
int		set_env_var(char ***envp, char	*var_name, char *value);
void	free_env(char **envp);
int		if_is_builtin(char *cmd);
int		is_echo_newline(char *current);
int		only_echo(t_cmds **cmds);
int		built_in(t_cmds **cmds, char ***env, char ***shell_envp, int *exit);
int		if_is_exit(t_cmds *cmds, int flag, int *exit);
int		exit_format(int i, t_cmds *cmds, int flag, int *exit);
int		echo(t_cmds *cmds);
int		change_dir(char **env, t_cmds *cmds);
int		cd_old(char **env, t_cmds *cmds);
int		go_to_user(t_cmds *cmds, char *dir, char **env);
void	change_old(char **env);
char	*get_path(t_cmds *cmds);
void	change_current_pwd(char **env);
char	*go_home(char **env);
int		get_env(t_cmds *cmds, char **env);
int		build_pwd(t_cmds *cmds);
int		pwd_goes_void(char **env, t_cmds *cmds);
int		is_void_pwd(t_cmds *cmds);
int		count_env_vars(char **envp);
void	export(char **cmds, char ***env, char ***shell_env);
char	*ft_strdup2(const char *s1, int stop);
void	here_doc(t_token *token, t_cmds *cmds);
int		is_inside_env(char	**envp, char *var_name, int count);
char	*get_next_var(char *var, char **envp);
char	*remove_char_from_word(char *str, char c);
char	*put_dollar_back(char *str);
char	*next_var(char *str, char	*real_str, int *i);
void	free_after_split(char **str);
int		wait_process(int last_pid, int last);
int		init_pipes(t_cmds **cmds, int index);
void	close_all(t_cmds **cmds);
void	free_everything(\
		t_cmds **red, char ***envp, char ***shell_env, t_tokens *tokens);
int		wait_process(int last_pid, int last);
void	close_pipes(t_cmds **red);
void	close_pipes_and_fds(t_cmds **red);
void	open_files_in_pipes(t_cmds **red);
void	dup_pipes(t_cmds **red, t_cmds **all);
void	print_string(t_cmds **red, char *str);
int		exec_full(\
			t_cmds **red, char ***envp, char ***shell_env, t_tokens *tokens);
int		execution(t_array_cmds cmds, \
			char ***envp, char ***shell_env, t_tokens *tokens);
int		run_if_builtin(t_cmds **red, char ***envp, char ***shell_env);
int		run_if_more_builtins(\
			t_array_cmds red, char ***shell_env, t_tokens *tokens);
char	*replace_var(char *var);
char	*put_dollar_back(char *str);
char	*next_dollar(char *str);
void	replace_environ_var(t_cmds **cmds, char **envp, int i, int j);
char	*replace_command(char *cmd, char *arg2, char **envp, int sum);
char	*replace_not_env_var(char *variable, char **final);
char	*create_dollar_str(char *final, char *cmds, int dollars, char *new_val);
char	*replace_var(char *var);
char	*put_dollar_back(char *str);
char	*next_dollar(char *str);
int		is_assign(char *str);
char	*remove_spaces(char *str, int *identify, int end_with_space, int i);
int		count_dollars(char *word);
void	free_env(char **envp);
char	*get_next_var(char *var, char **envp);
void	replace_if_quote(char **final, char *start);
void	replace_if_not_quote(char **final, char *start, char **envp);
void	replace_with_exit_c(char **final);
char	*next_var(char *str, char *real_str, int *i);
void	stop_if_any_quote(char	**real_str, int *i);
void	stop_if_quote_type(char	**real_str, int quote_type, int *i);
int		check_quote_type(char **real_str, int *i);
char	*remove_char_from_word(char *str, char c);
int		execute(char **envp);
void	check_grammar(t_tokens *tokens, \
			char ***envp, char ***shell_env, int *exit);
int		get_input(char **input);
int		execute_each_cmd(t_tokens *tokens, char ***envp, char ***shell_env);
int		execute_cmds(\
			t_cmds **cmds, char ***envp, char ***shell_env, t_tokens *tokens);
int		replace_cmds(t_cmds **cmds, int i);
int		free_the_environ(char **env, int last_pid);
char	**expanded_commands(t_cmds *cmds, int *k);
char	**copy_env(char **envp);
void	free_list_cmds(t_cmds **red, char ***envp, char ***shell_env);
int		count_builtins(t_cmds **red);
void	check_if_file_exists(t_cmds *cmds);
void	free_val(char *new_val);
void	init_values(int *i, int *last_pid, int *prev_pid);
void	init_export_vals(char **name, char **val, int *tmp);
int		no_input(t_cmds **cmds);
char	*escape_dquote(char	*cmds);
int		count_all_dollars(char *word);
int		has_a_dollar(char *str);
void	free_vars(char **variable, char **new_val);
#endif