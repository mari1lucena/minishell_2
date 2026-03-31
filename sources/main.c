/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danuno-g <danuno-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:52:17 by made-jes          #+#    #+#             */
/*   Updated: 2026/03/20 23:26:11 by danuno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_shell	*get_shell(void)
{
	static t_shell	shell;

	return (&shell);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	init_shell(envp);
	setup_signals();
	run_prompt();
	free_ast(get_shell()->ast);
	get_shell()->ast = NULL;
	free_token_list(get_shell()->tokens);
	get_shell()->tokens = NULL;
	free(get_shell()->line);
	get_shell()->line = NULL;
	free_env_list(get_shell()->env);
	rl_clear_history();
	return (get_shell()->last_exit);
}
