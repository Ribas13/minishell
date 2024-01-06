/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:08:59 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 21:21:53 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <dirent.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>

# define SUCCESS 0
# define FAILURE -1
# define OPERATORS "|<>"
# define QUOTES "\"\'"
# define DELIMS "\"\'"
# define REDIRECTS "><"

# define NOT_FOUND 0

// Errors

# define CL_ARGUMENTS_ERR "minishell: no support for command-line arguments"
# define NO_PIPE_PROMPT "minishell: no support for pipe prompt"
# define NO_SYTX_PROMPT "minishell: no support for syntax prompt"
# define PIPE_ERR "minishell: pipe() failed"
# define FORK_ERR "minishell: fork() failed"
# define UNCLOSED_QUOTES "minishell: unclosed quotes"
# define UNEXPECTED_TOKEN "minishell: syntax error near unexpected token `"
# define SYTX_ERR_RDR "minishell: syntax error near unexpected token `newline`"
# define EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments"
# define EXIT_NON_NUMERIC_ARG "minishell: exit: numeric argument required"
# define PATH_MAX 4096
# define CD_TOO_MANY_ARGS "minishell: cd: too many arguments"
# define OLDPWD_NOT_SET "minishell: cd: OLDPWD not set"

/* Structs */

typedef enum e_operator
{
	NONE,
	RDR_OUT_REPLACE,
	RDR_OUT_APPEND,
	RDR_INPUT,
	RDR_INPUT_UNTIL,
	PIPE,
}						t_operator;

//parser
typedef struct s_statement
{
	int					argc;
	char				**argv;
	t_operator			operator;
	struct s_statement	*next;
	bool				has_error;
	bool				has_out_rdr;
	bool				has_in_rdr;
	bool				previous_has_out_rdr;
}						t_statement;

//variables linked list
typedef struct s_vlst
{
	char				*var_name;
	char				*var_value;
	bool				is_exported;
	struct s_vlst		*next;
}						t_vlst;

//has the pointer to the head in case of fork or pipe error
typedef struct s_data
{
	char				**envp;
	t_vlst				*envp_lst;
	t_statement			*head;
	bool				parent;
	int					default_stdin;
	int					default_stdout;
}						t_data;

//Prototypes

//main.c

void		panic(t_data *data, char *error_msg, int exit_status);
void		print_lst(t_statement *node);

//setup_shell.c

void		setup_shell(char **envp, t_data *data,
				t_statement **statement_list);

//config_signals.c

void		dismiss_signal(int signum);
void		config_signals(void);
void		ft_handler(int signum);
t_data		*ft_data(void);

	/*	UTILS */

//cd_utils.c

int			cd_too_many_args(void);

//cmd_utils.c

void		cmd_not_found(char *cmd_name);

//destroy.c

void		destroy(t_data *data);
void		v_lstclear(t_vlst **head);
void		close_all_fds(int pipe[2]);

//init_envp_lst.c

t_vlst		*init_envp_lst(char **envp);

//invalid_syntax_utils.c

int			value_of_i(char input_i, char input_i_plus_1);

//is_all_digits_or_signals.c

bool		is_all_digits_or_signals(char *str);

//valid_input.c

bool		valid_input(char *input, t_data *data);

//fits_longlong.c

bool		fits_longlong(char *str);

//free_matrix.c

void		free_matrix(char **matrix);

//ft_atoll.c

long long	ft_atoll(const char *str);

//ft_lltoa.c

char		*ft_lltoa(long long n);

//is_onstr.c

bool		is_onstr(const char *str, int ch);

//is_spaces.c

bool		is_spaces(char c);

//linked lists (p_llst_utils.c, v_llst_utils.c, v_llst_utils_v2.c)

int			save_user_vars(char *statement, t_vlst **head, bool to_export);
char		**split_envp(char *env);
bool		get_exported_state(char *var_name, t_vlst **head);
size_t		v_lstsize_exported(t_vlst **head);
void		v_lstadd_back(t_vlst **head, t_vlst *new);
t_vlst		*v_lstlast(t_vlst *node);
t_vlst		*v_new_node(char *var_name, char *var_value, bool is_exported);
char		*get_var_value(char *var_name, t_data *data);
char		*get_fromvlst(char *var_name, t_vlst **head);
t_statement	*p_new_node(int ac);
size_t		p_lstsize(t_statement *head);
void		p_lstclear(t_statement **head);
void		p_lst_delone(t_statement *node);

//streq.c

bool		streq(char *str1, char *str2);

//trim_free.c

char		*trim_free(char *s1, char const *set);

//join_free.c

char		*join_free(char *s1, char *s2);

	/* CMDS */

//built-ins

bool		builtin(t_statement *s, t_data *data);
bool		is_valid_id(char *str);

//binaries.c

void		cmd_binaries(t_statement *statement, t_data *data);
bool		is_absolute_path(t_statement *statement);

//cd.c

int			cmd_cd(char *path, t_data *data);

//echo.c

int			cmd_echo(t_statement *statement, bool has_n);

//env.c

int			cmd_env(t_data *data);

//exit.c

void		cmd_exit(t_statement *s, t_data *data);

//export.c

int			cmd_export(t_statement *statement, t_data *data);
bool		is_onid(char *str, char c);

//pwd.c

int			cmd_pwd(void);

//unset.c

int			cmd_unset(t_statement *s, t_vlst **head);
int			unset_var(char *var_name, t_vlst **head);

	/* execs */

//exec_cmd.c

void		exec_type(t_statement *statement_list, t_data *data);
void		exec_cmd(t_statement *curr_node, t_data *data);

//executables.c

void		exec_executables(t_statement *node, t_data *data);

//pipe.c

void		exec_pipe(t_statement *node, t_data *data);

//redirects.c (Still needs explanation)

void		exec_redirects(t_statement *node, t_data *data);
t_statement	*jump_to_endcmd(t_statement *node);

//redirects_utils.c

char		*get_last_delim(t_statement *node);
bool		is_last_delim(t_statement *node, char *delim);
int			has_input_rdr(t_statement *node);
bool		has_heredoc(t_statement *node);
void		fake_heredoc(t_statement *node);

//redirect_utils2.c

t_statement	*get_last_infile(t_statement *node);
void		check_files(t_statement *node);
bool		has_output_redir(t_statement *node);

//rdr_inputs.c

void		set_input(t_statement *node);

//rdr_outputs.c

void		set_output(t_statement *node);
bool		is_last_out_rdr(t_statement *node, t_statement *nd_to_check);

	/* expander */

//expander.c (Still needs explanation)

char		*expander(char *input, t_data *data);
size_t		expand_variable(char *expanded_input_at_i, char *input,
				size_t *i, t_data *data);
size_t		expand_exit_status(char *expanded_input_at_i, size_t *i);

//expander_utils.c (Still needs explanation)

int			expanded_size(char *input, t_data *data);
size_t		expand_size(char *input_at_i, size_t *i, t_data *data);
size_t		exit_status_size(void);
void		init_vars(size_t *i, size_t *size, bool *in_quotes,
				bool *in_dquotes);

	/* parser */

//invalid_syntax.c

bool		invalid_syntax(char *input);
bool		invalid_syntax2(char *input);
bool		invalid_syntax_on_operator(char *input);
bool		has_operator(char *input);
bool		unexpected_token(char token);

//parser_utils.c

size_t		get_nr_statements(char *input);
size_t		get_token_len(char *input_at_i);
size_t		get_ac(char **parsed);
t_operator	get_operator(char *operator);

//parser.c

t_statement	*parser(char *input);
char		**parse_input(char *input);
void		join_args(t_statement *head);

//remove_quotes.c

char		*remove_quotes(char *parsed);
int			unclosed_quotes(char *str);

#endif