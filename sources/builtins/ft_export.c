/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:07:36 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/18 01:37:01 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_export_arg(t_shell *shell, char *arg)
{
	int		has_equal;
	t_env	*node;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	if (!is_valid_identifier(arg))
		return (ft_putstr_fd("minishell: export: `", 2),
			ft_putstr_fd(arg, 2),
			ft_putstr_fd("': not a valid identifier\n", 2), 1);
	if (!parse_export_arg(arg, &key, &value, &has_equal))
		return (1);
	if (key[0] == '\0')
		return (free(key), free(value), 0);
	if (!has_equal)
	{
		node = get_node_env(shell->env, key);
		if (!node)
			add_env(&shell->env, key, NULL);
	}
	else
		add_or_update(&shell->env, key, value);
	return (free(key), free(value), 0);
}

int	ft_export(t_shell *shell, char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_export_sorted(shell->env);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(shell, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || str[0] == '=')
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
