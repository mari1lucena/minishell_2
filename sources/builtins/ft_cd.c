/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:06:38 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/18 01:28:25 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*resolve_cd_target(t_shell *shell, char *arg)
{
	if (!arg || ft_strncmp(arg, "~", 2) == 0)
		return (get_env_value_from_env(shell->env, "HOME"));
	if (ft_strncmp(arg, "-", 2) == 0)
		return (get_env_value_from_env(shell->env, "OLDPWD"));
	return (arg);
}

static void	update_cd_env(t_shell *shell, char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (oldpwd)
		add_or_update(&shell->env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		add_or_update(&shell->env, "PWD", cwd);
}

static int	exec_cd(t_shell *shell, char *target, char *oldpwd, char **args)
{
	char	cwd[PATH_MAX];

	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (free(oldpwd), 1);
	}
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0 && getcwd(cwd, sizeof(cwd)))
		printf("%s\n", target);
	update_cd_env(shell, oldpwd);
	return (free(oldpwd), 0);
}

int	ft_cd(t_shell *shell, char **args)
{
	char	*oldpwd;
	char	*target;

	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	oldpwd = get_env_value_from_env(shell->env, "PWD");
	if (oldpwd)
		oldpwd = ft_strdup(oldpwd);
	target = resolve_cd_target(shell, args[1]);
	if (!target)
	{
		ft_putstr_fd("minishell: cd: No such file or directory\n", 2);
		return (free(oldpwd), 1);
	}
	return (exec_cd(shell, target, oldpwd, args));
}
