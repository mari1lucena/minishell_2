/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:06:41 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/08 00:32:17 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_env_var(char *entry)
{
	t_env	*node;
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return ;
	node = malloc(sizeof(t_env));
	if (!node)
		return ;
	node->key = ft_substr(entry, 0, eq - entry);
	node->value = ft_strdup(eq + 1);
	node->next = get_shell()->env;
	get_shell()->env = node;
}

static t_env	*create_env_node(char *env_str, char *eq)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_substr(env_str, 0, eq - env_str);
	node->value = ft_strdup(eq + 1);
	node->next = NULL;
	return (node);
}

t_env	*convert_envp_to_list(char **envp)
{
	t_env	*head;
	t_env	*last;
	t_env	*node;
	char	*eq;
	int		i;

	head = NULL;
	last = NULL;
	i = -1;
	while (envp[++i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
			continue ;
		node = create_env_node(envp[i], eq);
		if (!node)
			return (free_env_list(head), NULL);
		if (!head)
			head = node;
		else
			last->next = node;
		last = node;
	}
	return (head);
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
}

char	*handle_dollar_quote(char *res, char *str, int *i)
{
	*i += 2;
	while (str[*i] && str[*i] != '"')
		res = ft_strjoin_char_free(res, str[(*i)++]);
	if (str[*i] == '"')
		(*i)++;
	return (res);
}
