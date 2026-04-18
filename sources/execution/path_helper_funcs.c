/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_helper_funcs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:20:09 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/18 18:19:41 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*check_special_cmd(char *cmd, t_shell *shell)
{
	if (!ft_strncmp(cmd, ".", 2))
	{
		ft_putstr_fd("minishell.: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		shell->last_exit = 2;
		free_env_list(get_shell()->env);
		free_ast(get_shell()->ast);
		free_token_list(get_shell()->tokens);
		free(get_shell()->line);
		return ((char *)-1);
	}
	if (!ft_strncmp(cmd, "..", 3))
	{
		shell->last_exit = 127;
		free_env_list(get_shell()->env);
		free_ast(get_shell()->ast);
		free_token_list(get_shell()->tokens);
		free(get_shell()->line);
		return ((char *)-1);
	}
	return ((char *)1);
}

static char	*join_path_cmd(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

static char	*search_path(char *cmd, t_shell *shell)
{
	char	*env_path;
	char	**paths;
	char	*full_path;
	int		i;

	env_path = get_env_value_from_env(shell->env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*find_path(char *cmd, t_shell *shell)
{
	char	*special;

	special = check_special_cmd(cmd, shell);
	if (special == (char *)-1)
		return ((char *)-1);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (search_path(cmd, shell));
}
