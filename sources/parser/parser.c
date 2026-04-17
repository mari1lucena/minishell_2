/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:00:48 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/17 16:52:36 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(char *token)
{
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n",
		token);
	get_shell()->last_exit = 2;
	return (1);
}

static int	validate_token_syntax(t_token *tokens, t_token *prev)
{
	if (tokens->type == PIPE)
	{
		if (!prev || prev->type == PIPE)
			return (syntax_error("|"));
		if (!tokens->next || tokens->next->type == PIPE)
			return (syntax_error("|"));
	}
	if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
		|| tokens->type == APPEND || tokens->type == HEREDOC)
	{
		if (!tokens->next)
			return (syntax_error("newline"));
		if (!tokens->next || (tokens->next->type != WORD
				&& tokens->next->type != STR))
			return (syntax_error(tokens->next->value));
	}
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (validate_token_syntax(tokens, prev))
			return (1);
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}
