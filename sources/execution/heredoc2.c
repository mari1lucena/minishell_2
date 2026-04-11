/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:29:19 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/11 22:36:31 by made-jes         ###   ########.fr       */
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
