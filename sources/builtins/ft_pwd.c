/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:07:43 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/11 23:29:22 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(void)
{
	char	*pwd_env;
	char	cwd[PATH_MAX];

	pwd_env = get_env_value("PWD");
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putstr_fd(PWD_ERROR, 2);
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}
