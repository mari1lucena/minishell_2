/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 21:43:48 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/20 21:43:48 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_double_operator(const char *line, int *i)
{
	char	*op;

	op = ft_substr(line, *i, 2);
	*i += 2;
	return (op);
}

char	*handle_single_operator(const char *line, int *i)
{
	char	*op;

	op = ft_substr(line, (*i)++, 1);
	return (op);
}

static void	advance_word(const char *line, int *i, char *quote)
{
	if (!*quote && (line[*i] == '\'' || line[*i] == '"'))
	{
		*quote = line[*i];
		(*i)++;
	}
	else if (*quote && line[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
	}
	else if (*quote == '"' && line[*i] == '\\' && line[*i + 1])
		*i += 2;
	else
		(*i)++;
}

char	*handle_word(const char *line, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = 0;
	while (line[*i] && (quote || (!ft_is_space(line[*i])
				&& line[*i] != '>' && line[*i] != '<' && line[*i] != '|')))
		advance_word(line, i, &quote);
	if (quote)
		return (syntax_error("unclosed quotes"), NULL);
	return (ft_substr(line, start, *i - start));
}
