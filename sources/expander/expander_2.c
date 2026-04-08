/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 21:44:37 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/20 21:44:37 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	var_len(const char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}

static char	*expand_exit_status(void)
{
	return (ft_itoa(get_shell()->last_exit));
}

static char	*handle_braced_var(char *res, char *str, int *i)
{
	char	*key;
	char	*val;
	int		start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '}')
		(*i)++;
	key = ft_substr(str, start, *i - start);
	if (str[*i] == '}')
		(*i)++;
	val = get_env_value(key);
	if (val)
		res = ft_strjoin_free(res, val);
	free(key);
	return (res);
}

char	*expand_var(char *res, char *str, int *i)
{
	char	*key;
	char	*val;
	int		len;

	if (str[*i] == '?')
	{
		val = expand_exit_status();
		res = ft_strjoin_free(res, val);
		free(val);
		(*i)++;
		return (res);
	}
	if (str[*i] == '{')
		return (handle_braced_var(res, str, i));
	len = var_len(&str[*i]);
	if (len == 0)
		return (ft_strjoin_char_free(res, '$'));
	key = ft_substr(str, *i, len);
	val = get_env_value(key);
	if (val)
		res = ft_strjoin_free(res, val);
	free(key);
	*i += len;
	return (res);
}

char	*expand_dollar(char *str)
{
	char	*res;
	int		i;

	if (!str)
		return (ft_strdup(""));
	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			res = expand_var(res, str, &i);
		}
		else
			res = ft_strjoin_char_free(res, str[i++]);
	}
	return (res);
}
