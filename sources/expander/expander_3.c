/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:46:43 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/14 00:49:20 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_exit_status(void)
{
	return (ft_itoa(get_shell()->last_exit));
}

char	*expand_special_var(char *res, char *str, int *i)
{
	char	*val;

	if (str[*i] == '?')
	{
		val = expand_exit_status();
		res = ft_strjoin_free(res, val);
		free(val);
		(*i)++;
		return (res);
	}
	if (str[*i] == '$')
	{
		val = ft_itoa(get_shell()->pid_shell);
		res = ft_strjoin_free(res, val);
		free(val);
		(*i)++;
		return (res);
	}
	return (NULL);
}
