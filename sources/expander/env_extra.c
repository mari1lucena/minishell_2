/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_extra.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 00:19:33 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/18 21:40:33 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*create_shlvl_value(int shlvl)
{
	char	*new_shlvl;

	shlvl++;
	if (shlvl < 0)
		shlvl = 0;
	if (shlvl > 1000)
		shlvl = 1;
	new_shlvl = ft_itoa(shlvl);
	return (new_shlvl);
}

void	increment_shlvl(void)
{
	t_env	*env;
	int		shlvl;
	char	*new_entry;

	env = get_shell()->env;
	while (env)
	{
		if (ft_strncmp(env->key, "SHLVL", 5) == 0)
		{
			shlvl = ft_atoi(env->value);
			new_entry = create_shlvl_value(shlvl);
			if (new_entry)
			{
				free(env->value);
				env->value = new_entry;
			}
			return ;
		}
		env = env->next;
	}
	add_env_var("SHLVL=1");
}

void	init_shell(char **envp)
{
	t_shell	*sh;

	sh = get_shell();
	sh->env = NULL;
	if (envp && envp[0])
	{
		sh->env = convert_envp_to_list(envp);
		increment_shlvl();
	}
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
