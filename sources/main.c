/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:52:17 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/18 21:27:27 by made-jes         ###   ########.fr       */
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
	(void)av;
	if (ac == 1)
	{
		init_shell(envp);
		setup_signals();
		run_prompt();
		cleanup_and_exit(get_shell(), get_shell()->last_exit);
		return (1);
	}
	ft_putstr_fd("minishell: no arguments required\n", 2);
	return (0);
}
