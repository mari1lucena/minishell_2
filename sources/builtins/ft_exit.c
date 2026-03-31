/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlucena- <mlucena-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:07:13 by mlucena-          #+#    #+#             */
/*   Updated: 2026/03/28 12:52:24 by mlucena-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

int	ft_isnumeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exit_overflow(char *str)
{
	int		negative;
	size_t	len;

	negative = 0;
	if (*str == '-' || *str == '+')
		negative = (*str++ == '-');
	while (*str == '0')
		str++;
	len = ft_strlen(str);
	if (len == 0)
		return (1);
	if (len > 19)
		return (0);
	if (len < 19)
		return (1);
	if (negative)
		return (ft_strncmp(str, EXIT_ABS_MIN, 20) <= 0);
	else
		return (ft_strncmp(str, EXIT_MAX, 20) <= 0);
}

int	ft_exit(t_shell *shell, char **args)
{
	long	exit_code;

	exit_code = 0;
	if (args[1])
	{
		if (!ft_isnumeric(args[1]) || !exit_overflow(args[1]))
		{
			printf("exit\n");
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exit_code = 2;
			exit(exit_code);
		}
		else if (args[2])
		{
			printf("exit\n");
			fprintf(stderr, "exit: too many arguments\n");
			return (1);
		}
		else
			exit_code = (unsigned char)ft_atoi(args[1]);
	}
	printf("exit\n");
	cleanup_and_exit(shell, exit_code);
	exit(exit_code);
}
