/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/26 13:50:14 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		env(t_minishell *ms)
{
	char *env_list;
	int i = 0;

	while (ms->envp[i])
	{
		env_list = ft_strjoin(env_list, ms->envp[i]);
		env_list = add_char_to_word(env_list, '\n');
		i++;
	}
	printf("Envlist : %s\n", env_list);
	return (SUCCESS);
}
