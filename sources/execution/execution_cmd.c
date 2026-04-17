/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:03:43 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/17 23:25:00 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	save_stds(int fds[2])
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
}

void	exec_cmd_for_builtin(t_ast *node, int *fds_sup, t_shell *shell,\
	int in_pipe)
{
	int	is_exit;

	if (apply_redirecs(node->redirs))
	{
		if (!in_pipe)
			restore_stds(fds_sup);
		shell->last_exit = 1;
		return ;
	}
	is_exit = (ft_strncmp(node->cmd_args[0], "exit", 5) == 0);
	if (is_exit)
	{
		close(fds_sup[0]);
		close(fds_sup[1]);
	}
	shell->last_exit = exec_builtin(shell, node->cmd_args);
	if (!in_pipe && !is_exit)
		restore_stds(fds_sup);
}

static void	write_status(int exit_code)
{
	if (exit_code == 131)
		write(1, "Quit\n", 5);
}

static void	fork_wait(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	pid_t	pid;
	int		exit_code;
	int		in_pipe;

	ign_signals();
	in_pipe = (fds[0] != STDIN_FILENO || fds[1] != STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
		exec_cmd_aux(node, fds, shell, fds_sup);
	if (pid > 0)
	{
		waitpid(pid, &exit_code, 0);
		if (WIFEXITED(exit_code))
			get_shell()->last_exit = WEXITSTATUS(exit_code);
		else if (WIFSIGNALED(exit_code))
		{
			if (WTERMSIG(exit_code) == SIGINT && !in_pipe)
				write(1, "\n", 1);
			get_shell()->last_exit = 128 + WTERMSIG(exit_code);
		}
		write_status(get_shell()->last_exit);
	}
	else if (pid < 0)
		perror("fork");
}

void	exec_cmd(t_ast *node, int *fds, t_shell *shell)
{
	int	fds_sup[2];
	int	in_pipe;

	in_pipe = (fds[0] != STDIN_FILENO || fds[1] != STDOUT_FILENO);
	save_stds(fds_sup);
	if (fds[0] != STDIN_FILENO)
		dup2(fds[0], STDIN_FILENO);
	if (fds[1] != STDOUT_FILENO)
		dup2(fds[1], STDOUT_FILENO);
	if (is_builtin(node))
		exec_cmd_for_builtin(node, fds_sup, shell, in_pipe);
	else
	{
		if (apply_redirecs(node->redirs))
		{
			restore_stds(fds_sup);
			shell->last_exit = 1;
			return ;
		}
		fork_wait(node, fds, shell, fds_sup);
		restore_stds(fds_sup);
	}
	restore_stds(fds_sup);
}
