/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:03:53 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/18 21:40:17 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_ast(t_ast *node, int *fds, t_shell *shell)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
		exec_pipe(node, fds, shell);
	else if (node->type == NODE_CMD)
		exec_cmd(node, fds, shell);
}

void	wait_for_all_children(t_shell *shell)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
	}
	(void)shell;
}

int	exec_cmd_aux(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	char	*path;
	char	**envp;

	if (!node->cmd_args || !node->cmd_args[0])
		return (cleanup_and_exit(shell, 0), 0);
	if (node->cmd_args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		restore_stds(fds_sup);
		get_shell()->last_exit = 127;
		cleanup_and_exit(shell, 127);
		return (127);
	}
	path = prepare_child(node, fds, shell, fds_sup);
	if (!path)
		return (get_shell()->last_exit = 127, 127);
	envp = env_array(shell->env);
	apply_redirecs(node->redirs);
	execve(path, node->cmd_args, envp);
	perror("execve");
	shell->last_exit = 1;
	free(path);
	cleanup_and_exit(shell, 1);
	return (0);
}
