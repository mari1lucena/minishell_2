/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 17:59:14 by made-jes          #+#    #+#             */
/*   Updated: 2025/11/30 17:59:14 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_sigint(int sig)
{
	(void)sig;
	if (!get_shell()->in_prompt)
		write(1, "\n", 1);
	if (get_shell()->in_prompt)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	get_shell()->last_exit = 130;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	ign_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_sigint(int sig)
{
	(void)sig;
	if (get_shell()->heredoc_tmp_file)
	{
		close(3);
		free(get_shell()->heredoc_tmp_file);
		get_shell()->heredoc_tmp_file = NULL;
	}
	cleanup_heredoc_child(get_shell());
	exit(130);
}

void	append_token_list(t_token **head, t_token *new_node)
{
	t_token	*current;

	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
