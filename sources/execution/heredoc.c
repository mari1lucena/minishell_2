/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:04:28 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/18 21:59:09 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_heredoc_warning(int line_count, char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line_count, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

static int	process_heredoc_input(int file, char *del, int should_expand)
{
	char	*line;
	int		line_count;

	line_count = 1;
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (print_heredoc_warning(line_count, del), line_count);
		if (ft_strncmp(line, del, strlen(del)) == 0
			&& strlen(line) == strlen(del))
			return (free(line), line_count);
		process_heredoc_line(file, line, should_expand);
		free(line);
		line_count++;
	}
}

void	here_doc_read(int file, char *del, int should_expand, char *filename)
{
	get_shell()->heredoc_tmp_file = filename;
	signal(SIGINT, heredoc_sigint);
	process_heredoc_input(file, del, should_expand);
	close(file);
	free(filename);
	get_shell()->heredoc_tmp_file = NULL;
}

int	here_doc_execution(t_redir *redir)
{
	int	filefd;

	if (!redir || !redir->heredoc_file)
		return (1);
	filefd = open(redir->heredoc_file, O_RDONLY);
	if (filefd < 0)
	{
		perror("heredoc");
		return (1);
	}
	dup2(filefd, STDIN_FILENO);
	close(filefd);
	unlink(redir->heredoc_file);
	free(redir->heredoc_file);
	redir->heredoc_file = NULL;
	return (0);
}
