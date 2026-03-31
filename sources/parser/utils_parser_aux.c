/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:00:48 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/20 21:44:08 by made-jes         ###   ########.fr       */
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
