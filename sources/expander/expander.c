/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 23:46:46 by made-jes          #+#    #+#             */
/*   Updated: 2025/11/19 23:46:46 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *key)
{
	t_env	*env;

	env = get_shell()->env;
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(env->key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	handle_split_token(t_token **new, t_token *current, char *exp)
{
	t_token	*lst;
	t_token	*new_node;
	t_token	*tmp;

	if (!exp || exp[0] == '\0')
		return ;
	lst = split_and_create_tokens(exp);
	if (lst)
	{
		tmp = lst;
		while (tmp)
		{
			tmp->type = current->type;
			tmp->was_quoted = current->was_quoted;
			tmp = tmp->next;
		}
		append_token_list(new, lst);
	}
	else
	{
		new_node = new_token(exp);
		new_node->was_quoted = current->was_quoted;
		new_node->type = current->type;
		append_token_list(new, new_node);
	}
}

static void	process_token(t_token **new, t_token *current, int split)
{
	char		*exp;
	t_token		*new_node;
	int			had_quoted_space;

	had_quoted_space = 0;
	exp = expand_without_split(current->value, &had_quoted_space);
	if (split && !had_quoted_space)
		handle_split_token(new, current, exp);
	else
	{
		new_node = new_token(exp);
		new_node->was_quoted = current->was_quoted;
		new_node->type = current->type;
		append_token_list(new, new_node);
	}
	free(exp);
}

static void	process_single_token(t_token **new, t_token *current, t_token *prev)
{
	t_token	*new_node;
	char	*stripped;

	if (current->type == WORD)
	{
		if (prev && prev->type == HEREDOC)
		{
			stripped = expand_token_value(current->value);
			new_node = new_token(stripped);
			new_node->was_quoted = current->was_quoted;
			new_node->type = current->type;
			free(stripped);
			append_token_list(new, new_node);
		}
		else
			process_token(new, current, !current->was_quoted);
	}
	else
		append_token_list(new, new_token(current->value));
}

void	expand_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*new;
	t_token	*prev;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	new = NULL;
	prev = NULL;
	while (current)
	{
		process_single_token(&new, current, prev);
		prev = current;
		current = current->next;
	}
	free_token_list(*tokens);
	*tokens = new;
	get_shell()->tokens = new;
}
