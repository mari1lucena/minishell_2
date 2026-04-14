/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_extra.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 00:19:33 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/15 00:40:02 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*create_shlvl_entry(int shlvl)
{
	char	*new_shlvl;
	char	*new_entry;

	shlvl++;
	if (shlvl < 0)
		shlvl = 0;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return (NULL);
	new_entry = ft_strjoin("SHLVL=", new_shlvl);
	free(new_shlvl);
	return (new_entry);
}

void	increment_shlvl_in_array(char **envp)
{
	int		i;
	int		shlvl;
	char	*new_entry;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			shlvl = ft_atoi(envp[i] + 6);
			new_entry = create_shlvl_entry(shlvl);
			if (new_entry)
			{
				free(envp[i]);
				envp[i] = new_entry;
			}
			return ;
		}
		i++;
	}
}

void	init_shell(char **envp)
{
	t_shell	*sh;

	sh = get_shell();
	sh->env = NULL;
	if (envp && envp[0])
		sh->env = convert_envp_to_list(envp);
	else
	{
		add_env_var("PATH=/usr/local/bin:/usr/bin:/bin");
		add_env_var("PWD=.");
	}
	sh->last_exit = 0;
	sh->fd = -1;
	sh->line = NULL;
	sh->tokens = NULL;
	sh->ast = NULL;
	sh->in_prompt = 0;
	sh->heredoc_tmp_file = NULL;
	sh->pid_shell = getpid();
}
