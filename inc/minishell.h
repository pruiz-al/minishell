/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paularuizalcarazgmail.com <paularuizalc    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:08:56 by sbenitez          #+#    #+#             */
/*   Updated: 2025/05/13 02:32:46 by paularuizal      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/inc/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>

# define WORD 0
# define SQ 1
# define DQ 2
# define PI 3
# define RR1 4
# define RR2 5
# define LR1 6
# define LR2 7

extern int	g_signal_flag;

typedef struct s_xpnd
{
	int		start;
	int		end;
	char	*var;
	char	*value;
}				t_xpnd;

typedef struct s_token
{
	char			*tkn;
	int				type;
	bool			join;
	bool			expand;
	struct s_token	*next;
}				t_token;

typedef struct s_cmd
{
	char			**args; // List args del cmd, cmd[0] = cmd
	char			*infile;
	char			*delimiter; // caracter delimitador para heredoc <<
	char			*outfile;
	bool			append; // hay que apendear cuando sea >>
	bool			hd;
	bool			is_btn;
	int				exit_status;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_shell
{
	char	*input;
	char	**env;
	int		exit_status;
	int		last_exit_st;
	t_token	*token;
	t_cmd	*cmd_lst;
}				t_shell;

//CHECK_SYNTAX.C

void	ft_pipe_syntax(void);
int		ft_check_pipe(t_shell *shell);
int		ft_check_redir(t_shell *shell);
void	ft_check_syntax(t_shell *shell);

// CLEAN.C //

void	ft_free_matrix(char **matrix);
void	ft_free_tknlst(t_token **token);
void	ft_free_cmdlst(t_cmd **cmd_lst);
void	ft_cleanup_shell(t_shell *shell);
void	ft_clean(char **matrix, t_token *token, t_shell *shell);

// CLEAN2.C

void	ft_free_cmd_args(t_cmd *cmd);
void	ft_free_cmd_files(t_cmd *cmd);

// COMMANDS_UTILS.C

int		ft_strcmp(char *s1, char *s2);
int		ft_count_args(t_token *token);
int		ft_isbuiltin(char *str);
int		ft_has_commands(t_shell *shell);

// DEQUOTIZE.C

char	*ft_remove_quotes(char *tkn);
void	ft_dequotize(t_shell *shell);

// EXPAND_EXITSTATUS.C

char	*ft_middle_case(char *token, char *dollar_pos, char *status_str);
void	ft_replace_start(char **token, char *status_str, char *dollar_pos);
void	ft_expand_exitstatus(t_shell *shell, t_token *token);

// EXPAND_UTILS.C //

int		ft_intstrchr(const char *s, int c);
int		ft_find_end(char *tkn);
t_xpnd	*ft_init_expand(void);
char	*ft_substr_malloc(const char *input, int start, int len);
void	ft_free_expand(t_xpnd *xpnd);

// EXPAND_VAR.C //

char	*ft_getenv(char **env, char *var);
int		ft_find_dollar(t_shell *shell);
void	ft_insert_exp(t_xpnd *xpnd, t_token *t);
void	ft_expand_token(t_shell *shell, t_token *token);
void	ft_expand_var(t_shell *shell);

// GET_COMMANDS.C //

void	ft_fill_cmd(t_shell *shell, t_token *token, t_token **next_token);
void	ft_process_token(t_shell *shell, t_token **temp);
int		ft_has_commands(t_shell *shell);
int		ft_get_commands(t_shell *shell);

// INIT_SHELL.C

t_shell	*ft_init_shell(char **env);

// LIST_UTILS.C

void	ft_addback_tkn(t_token **lst, char *tkn, int type);
void	ft_addback_cmd(t_cmd **lst);
void	ft_update_join(t_token **lst);

// MAIN.C

void	ft_minishell(t_shell *shell, char **env);

// PROCESS_REDIRECTION.C //

void	ft_process_redir(t_shell *shell, t_token *token, t_token **next_token);
void	ft_process_input(t_cmd *cmd, t_token *token);
void	ft_process_output(t_cmd *cmd, t_token *token);
void	ft_process_append(t_cmd *cmd, t_token *token);
void	ft_process_heredoc(t_cmd *cmd, t_token *token);

// SIGNALS.C

void	ft_handle_sigint(int signum);
void	ft_setup_signals(void);

// UTILS.C //

void	ft_exit_error(char *error);
void	*safe_malloc(size_t bytes);
void	ft_print_tokens(t_token *token);
char	**ft_copy_env(char **env);
void	ft_print_cmdlst(t_cmd *cmd_lst);

// TKNIZE_QUOTES.C //

void	ft_closed_error(char *s);
int		ft_dquote_tkn(t_shell *shell, int *i);
int		ft_squote_tkn(t_shell *shell, int *i);
int		ft_quotes(t_shell *shell, int *i);
void	ft_join_token(t_shell *shell);

// TOKENIZE.C

void	ft_redir_tkn(t_shell *shell, int *i);
void	ft_word_tkn(t_shell *shell, int *i);
void	ft_tokenize(t_shell *shell);

// EXECUTER.C

void    ft_check_exitstat(int status, t_cmd *cmd);
void	execute_builtin(t_shell *ms, t_cmd *cmd);
void    child_process(t_cmd *cmd, int prevfd, int pipefd[2], t_shell *ms, char **envp[]);
void    parent_process(pid_t pid, t_cmd *cmd, int *prevfd, int pipefd[2]);
void    ft_exec_commands(t_shell *ms, char **envp[]);

// EXECUTER_COMMAND.C

void	free_paths(char **paths, int i);
char	*ft_path(char *path, char **comm);
void	execute_command(t_cmd *cmd, char **env);

// REDIRECTIONS.C

int		ft_redir_heredoc(t_cmd *cmd);
int 	ft_redir_infile(char *infile);
int 	ft_redir_outfile(char *outfile, int append);
int		ft_redirections(t_cmd *cmd);

// EXEC_PWD.C

int 	exec_pwd(void);

// EXEC_EXIT.X

int		ft_isnum(char *str);
void	free_exit(t_shell *ms);
void	exec_exit(t_shell *ms, t_cmd *cmd);

#endif