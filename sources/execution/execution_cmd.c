/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marilins <marilins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:03:43 by mlucena-          #+#    #+#             */
/*   Updated: 2026/03/31 18:46:24 by marilins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	save_stds(int fds[2])
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
}


/*void	exec_cmd_for_builtin(t_ast *node, int *fds_sup, t_shell *shell)
{
	save_stds(fds_sup);

	// 🔥 aplica redirections antes de executar o builtin
	if (apply_redirecs(node->redirs))
	{
		restore_stds(fds_sup);
		shell->last_exit = 1;
		return ;
	}

	// executa o builtin com todos os args intactos
	shell->last_exit = exec_builtin(shell, node->cmd_args);

	restore_stds(fds_sup);
}*/

// Função para comandos built-in (echo, cd, export, etc.)
void exec_cmd_for_builtin(t_ast *node, int *fds_sup, t_shell *shell)
{
    // 1️⃣ Salva os stds originais para restaurar depois
    save_stds(fds_sup);

    // 2️⃣ Aplica redirecionamentos do comando antes de executá-lo
    //    Se aplicar redirecionamento falhar (ex: diretório não existe), retorna com erro
    if (apply_redirecs(node->redirs))
    {
        restore_stds(fds_sup);
        shell->last_exit = 1;
        return;
    }

    // 3️⃣ Executa o built-in com os argumentos corretos
    shell->last_exit = exec_builtin(shell, node->cmd_args);

    // 4️⃣ Restaura os stds originais
    restore_stds(fds_sup);
}
/*void	exec_cmd_for_builtin(t_ast *node, int *fds_sup, t_shell *shell)
{
	if (!ft_strncmp(node->cmd_args[0], "exit", 5))
		restore_stds(fds_sup);
	shell->last_exit = exec_builtin(shell, node->cmd_args);
	if (ft_strncmp(node->cmd_args[0], "exit", 5))
		restore_stds(fds_sup);
}*/

static void	write_status(int exit_code)
{
	if (exit_code == 131)
		write(1, "Quit\n", 5);
	if (exit_code == 2)
		write(1, "\n", 1);
}

static void	fork_wait(t_ast *node, int *fds, t_shell *shell, int fds_sup[2])
{
	pid_t	pid;
	int		exit_code;

	ign_signals();
	pid = fork();
	if (pid == 0)
		exec_cmd_aux(node, fds, shell, fds_sup);
	restore_stds(fds_sup);
	if (pid > 0)
	{
		waitpid(pid, &exit_code, 0);
		if (WIFEXITED(exit_code))
			get_shell()->last_exit = WEXITSTATUS(exit_code);
		else if (WIFSIGNALED(exit_code))
			get_shell()->last_exit = 128 + WTERMSIG(exit_code);
		write_status(get_shell()->last_exit);
	}
	else if (pid < 0)
		perror("fork");
}

/*void	exec_cmd(t_ast *node, int *fds, t_shell *shell)
{
	int		fds_sup[2];

	save_stds(fds_sup);
	if (apply_redirecs(node->redirs))
	{
		restore_stds(fds_sup);
		shell->last_exit = 1;
		return ;
	}
	if (is_builtin(node))
		return (exec_cmd_for_builtin(node, fds_sup, shell));
	fork_wait(node, fds, shell, fds_sup);
}*/

void    exec_cmd(t_ast *node, int *fds, t_shell *shell)
{
    int     fds_sup[2];

    // Guarda os originais APENAS se não for um builtin que vai ser executado 
    // num filho (como no caso dos pipes, onde já estamos num processo separado).
    // Mas para simplificar a tua árvore, vamos sempre guardar e restaurar aqui.
    save_stds(fds_sup);
    
    // Aplicamos as pipes se houver
    if (fds[0] != STDIN_FILENO)
        dup2(fds[0], STDIN_FILENO);
    if (fds[1] != STDOUT_FILENO)
        dup2(fds[1], STDOUT_FILENO);

    // Agora aplicamos os redirecionamentos DO COMANDO (que têm prioridade sobre pipes)
    if (apply_redirecs(node->redirs))
    {
        restore_stds(fds_sup);
        shell->last_exit = 1;
        return ;
    }

    if (is_builtin(node))
    {
        exec_cmd_for_builtin(node, fds_sup, shell);
    }
    else
    {
        // O fork_wait vai criar o filho, mas também precisamos restaurar o pai
        fork_wait(node, fds, shell, fds_sup);
        restore_stds(fds_sup);
    }
}