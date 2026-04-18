/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:29:19 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/18 21:59:03 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_heredoc_line(int file, char *line, int should_expand)
{
	char	*expanded_line;

	expanded_line = NULL;
	if (should_expand)
		expanded_line = expand_heredoc_line(line);
	if (expanded_line)
		write(file, expanded_line, ft_strlen(expanded_line));
	else
		write(file, line, ft_strlen(line));
	write(file, "\n", 1);
	free(expanded_line);
}

char	*create_tmp_filename(void)
{
	static int	i;
	char		*name;
	char		*num;
	char		*tmp;

	num = ft_itoa(i++);
	if (!num)
		return (NULL);
	tmp = ft_strjoin("/tmp/minishell_heredoc_", num);
	free(num);
	if (!tmp)
		return (NULL);
	name = tmp;
	return (name);
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
