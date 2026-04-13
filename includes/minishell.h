/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: made-jes <made-jes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:50:01 by made-jes          #+#    #+#             */
/*   Updated: 2026/04/14 00:52:55 by made-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../libft/libft.h"
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <limits.h>
# include <sys/stat.h>
# include <errno.h>

# define PATH_MAX 4096
# define EXIT_MAX "9223372036854775807"
# define EXIT_ABS_MIN "9223372036854775808"
# define PWD_ERROR "pwd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n"

typedef enum e_token_type
{
	WORD,
	STR,
	PIPE,
	REDIR_IN, // <
	REDIR_OUT, // >
	APPEND, // >>
	HEREDOC, // <<
	UNKNOWN
}				t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				was_quoted;
	struct s_token	*next;
}				t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}			t_node_type;

typedef struct s_redir
{
	int				type;
	char			*filename;
	int				expand;
	char			*delimiter;
	int				quoted;
	char			*heredoc_file;
	struct s_redir	*next;
}				t_redir;

typedef struct s_ast
{
	t_node_type		type;
	char			**cmd_args;
	t_redir			*redirs;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	t_env		*env;
	int			fd;
	int			last_exit;
	int			in_prompt;
	char		*heredoc_tmp_file;
	pid_t		pid_shell;
}				t_shell;

t_shell			*get_shell(void);
void			init_shell(char **envp);
void			run_prompt(void);

//Lexer
t_token			*lexer(char *line);
t_token			*new_token(char *value);
t_token			*create_list(char **result);
void			fill_tokens(char **result, const char *line);
t_token_type	get_token_type(char *line);
int				syntax_error(char *token);
char			*handle_word(const char *line, int *i);
char			*handle_single_operator(const char *line, int *i);
char			*handle_double_operator(const char *line, int *i);

//Expander
void			add_env_var(char *entry);
char			*get_env_value(char *key);
char			*expand_var(char *res, char *str, int *i);
void			expand_tokens(t_token **tokens);
void			append_token_list(t_token **head, t_token *new_node);
t_token			*split_and_create_tokens(char *expanded);
char			*expand_token_value(char *str);
char			*expand_without_split(char *str, int *had_quoted_space);
char			*handle_dollar_quote(char *res, char *str, int *i);
char			*expand_dollar(char *str);
char			*expand_special_var(char *res, char *str, int *i);

//Parsing
int				validate_syntax(t_token *tokens);
t_ast			*build_ast(t_token *tokens);
t_token			*find_last_pipe(t_token *tokens);
int				count_args(t_token *start, t_token *end);
void			fill_args(t_ast *node, t_token *start, t_token *end);
void			add_redir(t_ast *node, int type, char *filename, int expand);
t_ast			*build_ast_range(t_token *start, t_token *end);
t_token			*find_pipe_in_range(t_token *start, t_token *end);
t_ast			*parse_command_and_redirs(t_token *start, t_token *end);
void			parse_redirections(t_ast *node, t_token *start, t_token *end);

//Signals and commands to exit
void			setup_signals(void);
void			ign_signals(void);
void			heredoc_sigint(int sig);

//Freeing
void			free_split(char **arr);
void			free_token_list(t_token *tokens);
void			free_redirs(t_redir *redir);
void			free_ast(t_ast *node);
void			free_env_list(t_env *env);
void			cleanup_and_exit(t_shell *shell, int exit_code);

//Execution
void			exec_ast(t_ast *node, int *fds, t_shell *shell);
void			restore_stds(int fds[2]);
void			exec_cmd_for_builtin(t_ast *node, int *fds_sup,\
					t_shell *shell, int in_pipe);
void			exec_cmd_aux(t_ast *node, int *fds,\
					t_shell *shell, int fds_sup[2]);
void			exec_cmd(t_ast *node, int *fds, t_shell *shell);
void			exec_pipe(t_ast *node, int *fds, t_shell *shell);
void			wait_for_all_children(t_shell *shell);
int				apply_redirecs(t_redir	*redirs);
int				handle_what_redir(t_redir *redir);
int				redir_in(char *filename);
int				redir_out(char *filename);
int				redir_append(char *filename);
char			**env_array(t_env *env);
void			free_envp(char **envp);
int				is_builtin(t_ast *node);
int				exec_builtin(t_shell *shell, char **args);

//Builtins
int				ft_echo(char **args);
int				ft_env(t_shell *shell);
int				ft_exit(t_shell *shell, char **args);
int				ft_export(t_shell *shell, char **args);
int				ft_pwd(void);
int				ft_unset(t_shell *shell, char **args);
int				ft_cd(t_shell *shell, char **args);
void			print_export_sorted(t_env *env);
t_env			*get_node_env(t_env *env, char *key);
void			update_env(t_env *node, char *value);
void			add_env(t_env **env, char *key, char *value);
int				parse_export_arg(char *arg, char **key, char **value,
					int *has_equal);
char			*get_env_value_from_env(t_env *env, char *key);
char			*find_path(char *cmd, t_shell *shell);
void			add_or_update(t_env **env, char *key, char *value);
int				is_valid_identifier(char *str);

//HereDoc
void			cleanup_heredoc_child(t_shell *shell);
char			*expand_heredoc_line(char *line);
void			here_doc_read(int file, char *del, int should_expand,
					char *filename);
char			*create_tmp_filename(void);
int				prepare_here_doc_file(char **filename);
int				spawn_here_doc_reader(t_redir *redir, int file, char *filename);
int				finish_here_doc(t_redir *redir, char *filename, int status);
int				handle_here_doc(t_redir *redir);
int				handle_here_doc_tree(t_ast *ast);
int				here_doc_execution(t_redir *redir);
void			process_heredoc_line(int file, char *line, int should_expand);

#endif
