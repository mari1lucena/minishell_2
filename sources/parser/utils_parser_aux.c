/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:00:48 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/14 20:43:13 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*build_ast_range(t_token *start, t_token *end)
{
	t_token	*pipe;
	t_ast	*node;

	if (!start || start == end)
		return (NULL);
	pipe = find_pipe_in_range(start, end);
	if (pipe)
	{
		node = malloc(sizeof(t_ast));
		if (!node)
			return (NULL);
		node->type = NODE_PIPE;
		node->cmd_args = NULL;
		node->redirs = NULL;
		node->left = build_ast_range(start, pipe);
		node->right = build_ast_range(pipe->next, end);
		return (node);
	}
	return (parse_command_and_redirs(start, end));
}

t_ast	*build_ast(t_token *tokens)
{
	return (build_ast_range(tokens, NULL));
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
