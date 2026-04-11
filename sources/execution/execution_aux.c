/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 17:37:51 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/11 17:25:25 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_pipe_aux1(t_ast *node, int *fds, t_shell *shell, int *pipe_fd)
{
	int	new_fds[2];

	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	new_fds[0] = fds[0];
	new_fds[1] = pipe_fd[1];
	exec_ast(node->left, new_fds, shell);
	cleanup_and_exit(shell, shell->last_exit);
}

static void	exec_pipe_aux2(t_ast *node, int *fds, t_shell *shell, int *pipe_fd)
{
	int	new_fds[2];

	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	new_fds[0] = pipe_fd[0];
	new_fds[1] = fds[1];
	exec_ast(node->right, new_fds, shell);
	cleanup_and_exit(shell, shell->last_exit);
}

static void	handle_pipe_status(int status1, int status2, t_shell *shell)
{
	if ((WIFSIGNALED(status1) && WTERMSIG(status1) == SIGINT)
		|| (WIFSIGNALED(status2) && WTERMSIG(status2) == SIGINT))
		write(1, "\n", 1);
	if (WIFEXITED(status2))
		shell->last_exit = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		shell->last_exit = 128 + WTERMSIG(status2);
}

void	exec_pipe(t_ast *node, int *fds, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

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
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	handle_pipe_status(status1, status2, shell);
}

void	restore_stds(int fds[2])
{
	if (fds[0] != -1)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		fds[0] = -1;
	}
	if (fds[1] != -1)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		fds[1] = -1;
	}
}
