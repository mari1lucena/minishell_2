/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 09:15:14 by mlucena-          #+#    #+#             */
/*   Updated: 2026/04/04 13:21:53 by made-jes         ###   ########.fr       */
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

void	checker_path(char *path, t_ast *node, t_shell *shell)
{
	struct stat	st;

	if (!path)
	{
		ft_putstr_fd(node->cmd_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(shell, 127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(node->cmd_args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		cleanup_and_exit(shell, 126);
	}
}

void	exec_cmd_aux(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	char	*path;
	char	**envp;
	struct stat	st;
	int	i;

	i = 0;
	close(fds_sup[0]);
	close(fds_sup[1]);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	if (fds[0] != STDIN_FILENO)
		dup2(fds[0], STDIN_FILENO);
	if (fds[1] != STDOUT_FILENO)
		dup2(fds[1], STDOUT_FILENO);

	while (node->cmd_args[i] && node->cmd_args[i][0] == '\0')
		i++;
	if (!node->cmd_args[i])
	{
		shell->last_exit = 0;
		cleanup_and_exit(shell, 0);
	}
	path = find_path(node->cmd_args[0], shell);
	checker_path(path, node, shell);

	// verifica se é diretório
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: is a directory\n", 2);
		shell->last_exit = 126;
		cleanup_and_exit(shell, 126);
	}
	node->cmd_args = &node->cmd_args[i];
	if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		perror(path);
		cleanup_and_exit(shell, 126);
	}
	// verifica permissão de execução
	if (access(path, X_OK) != 0)
	{
		if (errno == EACCES)
			ft_putstr_fd("minishell: permission denied\n", 2);
		else
			ft_putstr_fd("minishell: command not found\n", 2);
		shell->last_exit = 126;
		cleanup_and_exit(shell, 126);
	}

	envp = env_array(shell->env);

	// aplica redirections (arquivo de saída/entrada)
	apply_redirecs(node->redirs);

	// executa comando
	execve(path, node->cmd_args, envp);
	perror("execve");
	shell->last_exit = 1;
	cleanup_and_exit(shell, 1);
}
