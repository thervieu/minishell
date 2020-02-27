/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/27 09:25:43 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		env(t_minishell *ms)
{
	char	*env_list;
	int		i;
	
	i = 0;
	env_list = NULL;
	while (ms->envp[i])
	{
		env_list = ft_strjoin(env_list, ms->envp[i]);
		env_list = add_char_to_word(env_list, '\n');
		i++;
	}
	ms->output = env_list;
	if(!ms->has_spec_uf)
		ft_printf("Envlist : \n%s\n", env_list);
	else
		return (TREAT);
	return (SUCCESS);
}
