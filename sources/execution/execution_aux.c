/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:37:51 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/28 17:37:54 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_pipe_aux1(t_ast *node, int *fds, t_shell *shell, int *pipe_fd)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (fds[0] != STDIN_FILENO)
		dup2(fds[0], STDIN_FILENO);
	exec_ast(node->left, fds, shell);
	cleanup_and_exit(shell, shell->last_exit);
}

static void	exec_pipe_aux2(t_ast *node, int *fds, t_shell *shell, int *pipe_fd)
{
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (fds[1] != STDOUT_FILENO)
		dup2(fds[1], STDOUT_FILENO);
	exec_ast(node->right, fds, shell);
	cleanup_and_exit(shell, shell->last_exit);
}

void	exec_pipe(t_ast *node, int *fds, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid1 = fork();
	if (pid1 == 0)
		exec_pipe_aux1(node, fds, shell, pipe_fd);
	pid2 = fork();
	if (pid2 == 0)
		exec_pipe_aux2(node, fds, shell, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid2, &status, 0);
	waitpid(pid1, NULL, 0);
	if (WIFEXITED(status))
		shell->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit = 128 + WTERMSIG(status);
}

void	restore_stds(int fds[2])
{
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
}
