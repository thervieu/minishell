/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/25 17:55:22 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static char		**get_cmd_arguments(t_minishell *ms)
{
	char	**rtn;
	int		cursor;

	rtn = NULL;
	cursor = ms->seq_cursor;
	add_word_to_tab(ms->treated[cursor], &rtn);
	if (ms->sequence[cursor + 1] == 2)
	{
		cursor++;
		while (ms->sequence[cursor] == 2)
			add_word_to_tab(ms->treated[cursor++], &rtn);
	}
	return (rtn);
}

void			treat_command(t_minishell *ms)
{
	/** &cd, &print_work_dir, &exit_minishell, &env, **/
	static int	(*cmd[7])(int argc, char **argv, char **envp) = {
		&echo_, &print_work_dir, &exit_minishell, &env, &cd, &export_, &unset};
	char **argv;

	(void)cmd;
	argv = NULL;
	if (ms->sequence[ms->seq_cursor] == 0
		&& (ms->isexecret = is_exec(ms)) == ERROR)
	{
		if ((ms->iscmdret = is_cmd(ms->treated[ms->seq_cursor])) != -1
			&& (ms->iscmdret >= 0 && ms->iscmdret <= 6))
		{
			argv = get_cmd_arguments(ms);
			ms->last_cmd_rtn = cmd[ms->iscmdret](get_double_char_tab_len(argv), argv, envp);
		}
		else if (ms->sequence[ms->seq_cursor] == 0
			&& ms->iscmdret == -1 && ms->treated[ms->seq_cursor][0])
				error_command(ms->treated[ms->seq_cursor], ms);
	}
}

static void		cmd_no_pipe(t_minishell *ms)
{
	int			saved_stdout;
	int			saved_stdin;
	int			fdinput;
	int			fdoutput;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	fdoutput = has_redir_output(ms, 0, ms->seq_cursor + 1, STDOUT_FILENO);
	fdinput = has_redir_input(ms, 0, ms->seq_cursor + 1, STDIN_FILENO);
	dup2(fdinput, STDIN_FILENO);
	dup2(fdoutput, STDOUT_FILENO);
	while (ms->seq_cursor < ms->treated_len && ms->treated[ms->seq_cursor])
	{
		treat_command(ms);
		ms->seq_cursor++;
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

void			treat_entry(t_minishell *ms)
{
	ms->treated_len = get_double_char_tab_len(ms->treated);
	get_sequence(ms->treated, &ms->sequence);
/*
** REORDER DEVRA CHECKER LA VALIDITER DES FILES DE REDIR
*/
	reorder_sequence(ms);
	while (ms->treated[ms->cursor])
		ms->has_pipe += (ms->sequence[ms->cursor++] == 6) ? 1 : 0;
	ms->has_pipe += (ms->has_pipe) ? 1 : 0;
	(ms->has_pipe == 0) ? cmd_no_pipe(ms) : cmd_has_pipe(ms, 0, 0, 0);
	free_double_char_tab(ms->treated);
	free(ms->sequence);
}

// ctrl-\" a gérer?
int				main(int ac, char **av, char **env)
{
	t_minishell	ms;
	int			ret;
	char		*pwd;
	int			cmd_ret;

	// DEAL WITH ./ms < file
	cmd_ret = 0;
	(void)ac;
	(void)av;
	ret = 1;
	put_beg();
	dup_double_char_tab(env, &envp);
	// envp = env;
	while (ret == SUCCESS)
	{
		ms = (t_minishell){.iscmdret = -1, .isexecret = -1, .last_cmd_rtn = cmd_ret};
		if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("\e[97m[\e[91mm\e[92mi\e[93mn\e[94mi\e[95ms\e[96mh\e[91me");
		ft_printf("\e[92ml\e[93ml\e[97m] \e[91m%s \e[97m: ", pwd);
		free(pwd);
		ret = line_edition(&ms);
		if (!sanitize(&ms, ms.entry, &ms.treated))
			return (0);
		treat_entry(&ms);
		cmd_ret = ms.last_cmd_rtn;
	}
	(void)cmd_ret;
	return (0);
}
