/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:04:20 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/10 23:01:31 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_heredoc_child(t_shell *shell)
{
	free_env_list(shell->env);
	free_ast(shell->ast);
	free_token_list(shell->tokens);
	free(shell->line);
	rl_clear_history();
}

char	*expand_heredoc_line(char *line)
{
	char	*expanded;

	if (!line)
		return (NULL);
	expanded = expand_dollar(line);
	return (expanded);
}

int	prepare_here_doc_file(char **filename)
{
	int	file;

	*filename = create_tmp_filename();
	if (!*filename)
		return (-1);
	file = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
	{
		free(*filename);
		*filename = NULL;
	}
	return (file);
}

int	spawn_here_doc_reader(t_redir *redir, int file, char *filename)
{
	int	pid;

	ign_signals();
	pid = fork();
	if (pid < 0)
	{
		close(file);
		unlink(filename);
		free(filename);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		here_doc_read(file, redir->filename, redir->expand);
		close(file);
		free(filename);
		cleanup_heredoc_child(get_shell());
		exit(0);
	}
	return (pid);
}

int	finish_here_doc(t_redir *redir, char *filename, int status)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		unlink(filename);
		free(filename);
		get_shell()->last_exit = 130;
		if (redir)
			redir->heredoc_file = NULL;
		return (-1);
	}
	if (redir->heredoc_file)
	{
		unlink(redir->heredoc_file);
		free(redir->heredoc_file);
	}
	redir->heredoc_file = filename;
	return (0);
}
