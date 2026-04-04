/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:33:21 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/04 17:25:33 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	freeing_stuff(char *line, t_token *tokens)
{
	free_ast(get_shell()->ast);
	get_shell()->ast = NULL;
	free_token_list(tokens);
	free(line);
}

void	process_ast(t_token *tokens)
{
	int	fds[2];

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	get_shell()->ast = build_ast(tokens);
	if (handle_here_doc_tree(get_shell()->ast))
	{
		get_shell()->last_exit = 1;
		return ;
	}
	exec_ast(get_shell()->ast, fds, get_shell());
	wait_for_all_children(get_shell());
	free_ast(get_shell()->ast);
	get_shell()->ast = NULL;
}

static int	process_line(char *line)
{
	t_token	*tokens;

	tokens = lexer(line);
	get_shell()->tokens = tokens;
	expand_tokens(&tokens);
	get_shell()->tokens = tokens;
	if (validate_syntax(tokens))
	{
		free_token_list(tokens);
		get_shell()->tokens = NULL;
		free(line);
		get_shell()->line = NULL;
		return (1);
	}
	process_ast(tokens);
	freeing_stuff(line, tokens);
	get_shell()->line = NULL;
	get_shell()->tokens = NULL;
	return (0);
}

void	run_prompt(void)
{
	char	*line;

	while (1)
	{
		setup_signals();
		line = readline("minishell$ ");
		get_shell()->line = line;
		if (get_shell()->sigint_received)
		{
			get_shell()->last_exit = 130;
			get_shell()->sigint_received = 0;
		}
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != 0)
			add_history(line);
		process_line(line);
	}
}
