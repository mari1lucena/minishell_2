/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 09:15:14 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/14 21:36:09 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_and_exit(t_shell *shell, int exit_code)
{
	free_env_list(shell->env);
	free_ast(shell->ast);
	free_token_list(shell->tokens);
	free(shell->line);
	rl_clear_history();
	exit(exit_code);
}

static void	exit_error(char *path, t_shell *shell, int code)
{
	free(path);
	cleanup_and_exit(shell, code);
}

void	checker_path(char *path, t_ast *node, t_shell *shell)
{
	struct stat	st;

	if (!path)
	{
		ft_putstr_fd(node->cmd_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(shell, 127);
	}
	if (access(path, F_OK) != 0)
	{
		perror(node->cmd_args[0]);
		exit_error(path, shell, 127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(node->cmd_args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit_error(path, shell, 126);
	}
	if (access(path, X_OK) != 0)
	{
		perror(node->cmd_args[0]);
		exit_error(path, shell, 126);
	}
}

char	*prepare_child(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	char	*path;

	close(fds_sup[0]);
	close(fds_sup[1]);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (fds[0] != STDIN_FILENO)
		dup2(fds[0], STDIN_FILENO);
	if (fds[1] != STDOUT_FILENO)
		dup2(fds[1], STDOUT_FILENO);
	while (*node->cmd_args && **node->cmd_args == '\0')
		node->cmd_args++;
	if (!*node->cmd_args)
		cleanup_and_exit(shell, (shell->last_exit = 0, 0));
	if (ft_strchr(node->cmd_args[0], '/'))
		path = ft_strdup(node->cmd_args[0]);
	else
		path = find_path(node->cmd_args[0], shell);
	if (path == (char *)-1)
		exit(shell->last_exit);
	checker_path(path, node, shell);
	return (path);
}

void	exec_cmd_aux(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	char	*path;
	char	**envp;

	path = prepare_child(node, fds, shell, fds_sup);
	envp = env_array(shell->env);
	apply_redirecs(node->redirs);
	execve(path, node->cmd_args, envp);
	perror("execve");
	shell->last_exit = 1;
	free(path);
	cleanup_and_exit(shell, 1);
}
