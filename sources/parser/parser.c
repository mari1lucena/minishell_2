/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marilins <marilins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:00:48 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/31 14:47:19 by marilins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(char *token)
{
	printf("minishell: syntax error near unexpected token `%s'\n", token);
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
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
			if (!tokens->next || (tokens->next->type != WORD
					&& tokens->next->type != STR))
				return (syntax_error(tokens->value));
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}

int	count_args(t_token *start, t_token *end)
{
	int		count;
	t_token	*tokens;

	count = 0;
	tokens = start;
	while (tokens && tokens != end)
	{
		if (tokens->type == WORD || tokens->type == STR)
			count++;
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == APPEND || tokens->type == HEREDOC)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	return (count);
}

void	fill_args(t_ast *node, t_token *start, t_token *end)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = start;
	while (tokens && tokens != end)
	{
		if (tokens->type == WORD || tokens->type == STR)
		{
			if (tokens->value[0] != '\0' || tokens->was_quoted)
				node->cmd_args[i++] = ft_strdup(tokens->value);
		}
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == APPEND || tokens->type == HEREDOC)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	node->cmd_args[i] = NULL;
}

void	add_redir(t_ast *node, int type, char *filename, int expand)
{
	t_redir	*redir;
	t_redir	*current;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = type;
	redir->filename = ft_strdup(filename);
	redir->expand = expand;
	redir->delimiter = NULL;
	redir->quoted = 0;
	redir->heredoc_file = NULL;
	redir->next = NULL;
	if (!node->redirs)
		node->redirs = redir;
	else
	{
		current = node->redirs;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}
