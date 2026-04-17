/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:07:13 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/18 00:07:02 by made-jes         ###   ########.fr       */
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

static long	handle_exit_args(t_shell *shell, char **args, int is_interactive)
{
	long	exit_code;
	
	if (!ft_isnumeric(args[1]) || !exit_overflow(args[1]))
	{
		if (is_interactive)
			printf("exit\n");
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		exit_code = 2;
		cleanup_and_exit(shell, exit_code);
	}
	if (args[2])
	{
		if (is_interactive)
			printf("exit\n");
		fprintf(stderr, "minishell: exit: too many arguments\n");
		shell->last_exit = 1;
		return (-1);
	}
	return ((unsigned char)ft_atoi(args[1]));
}

int	ft_exit(t_shell *shell, char **args)
{
	long	exit_code;
	int		is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	exit_code = 0;
	if (args[1])
	{
		exit_code = handle_exit_args(shell, args, is_interactive);
		if (exit_code == -1)
			return (1);
	}
	if (is_interactive)
		printf("exit\n");
	cleanup_and_exit(shell, exit_code);
	exit(exit_code);
}
