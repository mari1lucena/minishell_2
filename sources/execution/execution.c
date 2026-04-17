/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:03:53 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/17 20:16:17 by made-jes         ###   ########.fr       */
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

void	exec_cmd_aux(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	char	*path;
	char	**envp;

	path = prepare_child(node, fds, shell, fds_sup);
	envp = env_array(shell->env);
	increment_shlvl_in_array(envp);
	apply_redirecs(node->redirs);
	execve(path, node->cmd_args, envp);
	perror("execve");
	shell->last_exit = 1;
	free(path);
	cleanup_and_exit(shell, 1);
}
