/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlucena- <mlucena-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:06:47 by mlucena-          #+#    #+#             */
/*   Updated: 2026/03/28 12:56:55 by mlucena-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	n_flag(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

/*static void	check_for_sign(char **args, int *i)
{
	if (args[*i][0] == '$')
	{
		printf("%s", args[*i] + 1);
	}
	else
		printf("%s", args[*i]);
	if (args[*i + 1])
		printf(" ");
	(*i)++;
}*/

int	ft_echo(char **args)
{
	int	i;
	int	nl;

	if (!args || !args[0])
		return (0);
	i = 1;
	nl = 1;
	while (args[i] && n_flag(args[i]))
	{
		nl = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}
