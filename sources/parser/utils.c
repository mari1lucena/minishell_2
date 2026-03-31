/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marilins <marilins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:51:15 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/31 14:47:00 by marilins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*find_last_pipe(t_token *tokens)
{
	t_token	*last_pipe;

	last_pipe = NULL;
	while (tokens && tokens->next)
	{
		if (tokens->type == PIPE)
			last_pipe = tokens;
		tokens = tokens->next;
	}
	return (last_pipe);
}

void	parse_redirections(t_ast *node, t_token *start, t_token *end)
{
	int		should_expand;
	t_token	*tokens;

	tokens = start;
	while (tokens && tokens != end)
	{
		if ((tokens->type == REDIR_IN || tokens->type == REDIR_OUT
				|| tokens->type == APPEND || tokens->type == HEREDOC)
			&& tokens->next)
		{
			should_expand = 1;
			if (tokens->type == HEREDOC && tokens->next->was_quoted)
				should_expand = 0;
			add_redir(node, tokens->type, tokens->next->value, should_expand);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
}

t_ast	*parse_command_and_redirs(t_token *start, t_token *end)
{
	t_ast	*node;
	int		count;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->redirs = NULL;
	count = count_args(start, end);
	node->cmd_args = malloc(sizeof(char *) * (count + 1));
	if (!node->cmd_args)
	{
		free(node);
		return (NULL);
	}
	fill_args(node, start, end);
	parse_redirections(node, start, end);
	return (node);
}

t_token	*find_pipe_in_range(t_token *start, t_token *end)
{
	t_token	*pipe;
	t_token	*current;

	pipe = NULL;
	current = start;
	while (current && current != end)
	{
		if (current->type == PIPE && current->next != end)
			pipe = current;
		current = current->next;
	}
	return (pipe);
}
