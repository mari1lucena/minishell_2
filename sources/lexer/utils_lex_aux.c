/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:43:17 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/17 16:28:48 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_next_token(const char *line, int *i)
{
	if (line[*i] == '\'' || line[*i] == '"')
		return (handle_word(line, i));
	if ((line[*i] == '>' && line[*i + 1] == '>')
		|| (line[*i] == '<' && line[*i + 1] == '<'))
		return (handle_double_operator(line, i));
	else if (line[*i] == '>' || line[*i] == '<' || line[*i] == '|')
		return (handle_single_operator(line, i));
	else
		return (handle_word(line, i));
}

void	fill_tokens(char **result, const char *line)
{
	int		i;
	int		j;
	char	*token;

	i = 0;
	j = 0;
	while (line[i])
	{
		while (ft_is_space(line[i]))
			i++;
		if (line[i] == '\0')
			break ;
		token = get_next_token(line, &i);
		if (!token)
		{
			while (j > 0)
				free(result[--j]);
			result[0] = NULL;
			return ;
		}
		result[j++] = token;
	}
	result[j] = NULL;
}
