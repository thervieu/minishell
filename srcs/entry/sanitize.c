/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie </var/mail/rchallie>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 19:14:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/30 20:04:08 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:30:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/30 19:13:18 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		add_word_to_tab(char *word, char ***treated)
{
	char	**new_tab;
	char	**save_treated;
	int		treated_len = 0;
	int		i = 0;

	if (!word)
		return (SUCCESS);
	if (!treated || !*treated)
	{
		if (!(new_tab = (char **)malloc(sizeof(char *) * 2)))
			return (ERROR);
		ft_bzero(new_tab, sizeof(char *) * 2);
		*new_tab = ft_strdup(word);
		*treated = new_tab;
		return (SUCCESS);
	}
	save_treated = *treated;
	treated_len = get_double_char_tab_len(*treated);
	i = 0;
	if (!(new_tab = (char **)malloc(sizeof(char *) * (treated_len + 2))))
		return (ERROR);
	ft_bzero(new_tab, sizeof(char *) * (treated_len + 2));
	while (i < treated_len)
	{
		new_tab[i] = ft_strdup(treated[0][i]);
		i++;
	}
	new_tab[i] = ft_strdup(word);

	*treated = new_tab;
	free_double_char_tab(save_treated);
	return (SUCCESS);
}

static int		special_char(char ***treated, char *entry, int up, char c)
{
	char	*word;
	
	word = NULL;
	if (*(entry + up) == c)
	{
		if (word)
			free(word);
		word = NULL;
		while (*(entry + up) == c)
		{
			word = add_char_to_word(word, *(entry + up));
			up++;
		}
		if (ft_secure_strlen(word) > 2)
		{
			ft_printf("minishell: syntax error near unexpected token `%c%c'\n", c, c); // ||| = 1, >>/<</>/< = 2
			return (ERROR);
		}
		else
			add_word_to_tab(word, treated);
		while (ft_is_whitespace(*(entry + up)))
		up++;
	}
	return (up);
}

static int		check_special_chars(char ***treated, char *entry, int up)
{
	if (!(up = special_char(treated, entry, up, '>'))
		|| !(up = special_char(treated, entry, up, '<'))
		|| !(up = special_char(treated, entry, up, '|'))
		|| !(up = special_char(treated, entry, up, '>'))
		|| !(up = special_char(treated, entry, up, ';')))
			return (ERROR);
	return (up);
}

int				sanitize(t_minishell *ms, char *entry, char ***treated)
{
	char	*word;
	int		up;
	
	// printf("\nbef san entry = |%s|\n", entry);
	word = NULL;
	if (!entry || !*entry || entry[0] == '\n')
	{
		add_word_to_tab("\n", treated);
		free(entry);
		return (SUCCESS);
	}
	up = 0;
	while (*(entry + up))
	{
		while (ft_is_whitespace(*(entry + up)))
			up++;
		word = NULL;
		up += get_word(ms, (entry + up), &word);
		if (word && word[0] == '~' && ( !word[1] || word[1] == '/'))
		{
			char *home = get_env_var_by_name("HOME", envp);
			if (ft_secure_strlen(home) != 0)
				word = ft_strjoin(home, (word + 1));
			else
			{
				char *tmp_word = ft_strjoin("/Users/", get_env_var_by_name("USER", envp));
				word = ft_strjoin(tmp_word, (word + 1));
				free(tmp_word);
			}
			free(home);			
		}
		add_word_to_tab(word, treated);
		free(word);
		while (ft_is_whitespace(*(entry + up)))
			up++;
		if(!(up = check_special_chars(treated, entry, up)))
			return (ERROR);
		while (ft_is_whitespace(*(entry + up)))
			up++;
	}
	free(entry);
	add_word_to_tab("\n", treated);
	return (SUCCESS);
}
