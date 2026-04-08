/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:04:28 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/08 01:17:25 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	here_doc_read(int file, char *del, int should_expand)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, del, strlen(del)) == 0
			&& strlen(line) == strlen(del))
		{
			free(line);
			break ;
		}
		expanded_line = NULL;
		if (should_expand)
			expanded_line = expand_heredoc_line(line);
		if (expanded_line)
			write(file, expanded_line, ft_strlen(expanded_line));
		else
			write(file, line, ft_strlen(line));
		write(file, "\n", 1);
		free(expanded_line);
		free(line);
	}
}

char	*create_tmp_filename(void)
{
	char	*base;
	char	*num;
	char	*name;
	int		i;

	base = "/tmp/minishell_heredoc_";
	i = 0;
	while (i < 10000)
	{
		num = ft_itoa(i);
		if (!num)
			return (NULL);
		name = ft_strjoin(base, num);
		free(num);
		if (!name)
			return (NULL);
		if (access(name, F_OK) != 0)
			return (name);
		free(name);
		i++;
	}
	return (NULL);
}

int	handle_here_doc(t_redir *redir)
{
	int		pid;
	int		file;
	int		status;
	char	*filename;

	file = prepare_here_doc_file(&filename);
	if (file < 0)
		return (-1);
	pid = spawn_here_doc_reader(redir, file, filename);
	if (pid < 0)
		return (-1);
	waitpid(pid, &status, 0);
	close(file);
	return (finish_here_doc(redir, filename, status));
}

int	handle_here_doc_tree(t_ast *ast)
{
	t_redir	*aux;
	int		status;

	if (ast)
	{
		aux = ast->redirs;
		while (aux)
		{
			if (aux->type == HEREDOC)
			{
				status = handle_here_doc(aux);
				if (status != 0)
					return (1);
			}
			aux = aux->next;
		}
		if (handle_here_doc_tree(ast->left))
			return (1);
		if (handle_here_doc_tree(ast->right))
			return (1);
		return (0);
	}
	return (0);
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
