/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:04:56 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/18 12:54:45 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	apply_redirecs(t_redir *redirs)
{
	t_redir	*last_heredoc;
	t_redir	*temp;

	last_heredoc = NULL;
	temp = redirs;
	while (temp)
	{
		if (temp->type == HEREDOC)
			last_heredoc = temp;
		temp = temp->next;
	}
	while (redirs)
	{
		if (redirs->type != HEREDOC || redirs == last_heredoc)
		{
			if (handle_what_redir(redirs))
				return (1);
		}
		redirs = redirs->next;
	}
	return (0);
}

int	handle_what_redir(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (redir_in(redir->filename));
	else if (redir->type == REDIR_OUT)
		return (redir_out(redir->filename));
	else if (redir->type == APPEND)
		return (redir_append(redir->filename));
	else if (redir->type == HEREDOC)
		return (here_doc_execution(redir));
	return (0);
}

int	redir_in(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redir_out(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redir_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
