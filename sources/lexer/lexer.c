/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 21:25:38 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/18 12:30:44 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*lexer(char *line)
{
	char	**result;
	t_token	*token_list;

	if (!line)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_strlen(line) + 1));
	if (!result)
		return (NULL);
	fill_tokens(result, line);
	token_list = create_list(result);
	free_split(result);
	get_shell()->tokens = token_list;
	return (token_list);
}

t_token_type	get_token_type(char *line)
{
	if (!line)
		return (UNKNOWN);
	if (ft_strncmp(line, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(line, "<<", 3) == 0)
		return (HEREDOC);
	if (ft_strncmp(line, ">>", 3) == 0)
		return (APPEND);
	if (ft_strncmp(line, "<", 2) == 0)
		return (REDIR_IN);
	if (ft_strncmp(line, ">", 2) == 0)
		return (REDIR_OUT);
	return (WORD);
}

int	is_fully_quoted(char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 2)
		return (0);
	return ((s[0] == '\'' && s[len - 1] == '\'')
		|| (s[0] == '"' && s[len - 1] == '"'));
}

t_token	*new_token(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->was_quoted = is_fully_quoted(value);
	if (token->was_quoted)
		token->type = WORD;
	else
		token->type = get_token_type(value);
	token->next = NULL;
	return (token);
}

t_token	*create_list(char **result)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (result[i])
	{
		new = new_token(result[i]);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		i++;
	}
	return (head);
}
