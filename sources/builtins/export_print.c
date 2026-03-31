/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlucena- <mlucena-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:06:29 by mlucena-          #+#    #+#             */
/*   Updated: 2026/03/28 12:06:33 by mlucena-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*env_node_to_string(t_env *node)
{
	char	*res;
	char	*tmp;

	if (node->value)
	{
		tmp = ft_strjoin(node->key, "=");
		res = ft_strjoin(tmp, node->value);
		free(tmp);
	}
	else
		res = ft_strdup(node->key);
	return (res);
}

char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**arr;
	int		i;

	count = 0;
	tmp = env;
	i = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		arr[i++] = env_node_to_string(tmp);
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	sort_env_array(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i]) + 1) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_export_sorted(t_env *env)
{
	char	**arr;
	int		i;
	char	*eq;

	arr = env_to_array(env);
	if (!arr)
		return ;
	sort_env_array(arr);
	i = 0;
	while (arr[i])
	{
		eq = ft_strchr(arr[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("declare -x %s=\"%s\"\n", arr[i], eq + 1);
			*eq = '=';
		}
		else
			printf("declare -x %s\n", arr[i]);
		i++;
	}
	free_envp(arr);
}
