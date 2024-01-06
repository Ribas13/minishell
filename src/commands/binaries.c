/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 16:46:47 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/03 01:21:02 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern long long	g_exit_status;

#define NOT_FOUND NULL

bool	is_absolute_path(t_statement *statement)
{
	if (is_onstr(statement->argv[0], '/'))
		return (true);
	return (false);
}

/* Fetch the path from the envp list */
static char	**get_paths(t_vlst *envp_lst)
{
	t_vlst	*temp;

	temp = envp_lst;
	while (!streq("PATH", temp->var_name))
	{
		temp = temp->next;
		if (temp == NULL)
			return (NULL);
	}
	return (ft_split(temp->var_value, ':'));
}

/* Gets the binary path */
static char	*get_bin_path(char	*cmd, char **paths)
{
	size_t	i;
	char	*tmp;
	char	*bin_path;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		bin_path = join_free(tmp, cmd);
		if (access(bin_path, F_OK | X_OK) == 0)
			return (bin_path);
		free(bin_path);
		i += 1;
	}
	return (NULL);
}

/* Cleans resources and terminates the shell
(WIFSIGNALED checks to see if a child process ended abnormally) */
static void	exit_and_free_matrix(char **paths, char *cmd, int exit_status)
{
	if (cmd)
		perror(cmd);
	free_matrix(paths);
	if (!WIFSIGNALED(g_exit_status))
		g_exit_status = exit_status;
	exit(g_exit_status);
}

/* See if file exists, isn't a folder and have permission to run */
void	solve_access(char *path, char **paths)
{
	struct stat	file_type;

	if (!path)
		return ;
	stat(path, &file_type);
	if (access(path, F_OK))		//check if exists
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free (path);
		exit_and_free_matrix(paths, NULL, 127);
	}
	if (S_ISDIR(file_type.st_mode) || access(path, X_OK))	//check if it's a directory and permission to run
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		free (path);
		if (S_ISDIR(file_type.st_mode))
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit_and_free_matrix(paths, NULL, 126);
	}
}

/* Checks wether the binary path is absolute or relative, attempts
to execute the binary and frees resources */
void	cmd_binaries(t_statement *statement, t_data *data)
{
	char	*bin_path;
	char	**paths;

	paths = NULL;
	bin_path = ft_strdup(statement->argv[0]);
	if (!is_absolute_path(statement))
	{
		free(bin_path);
		paths = get_paths(data->envp_lst);
		bin_path = get_bin_path(statement->argv[0], paths);
		if (bin_path == NOT_FOUND)
		{
			cmd_not_found(statement->argv[0]);
			exit_and_free_matrix(paths, NULL, 127);
		}
	}
	solve_access(bin_path, paths);
	if (execve(bin_path, statement->argv, data->envp) == -1)
	{
		free(bin_path);
		exit_and_free_matrix(paths, statement->argv[0], 127);
	}
	free(bin_path);
	exit_and_free_matrix(paths, NULL, EXIT_SUCCESS);
}

/* void	cmd_binaries(t_statement *statement, t_data *data)
{
	char	*bin_path;
	char	**paths;

	bin_path = statement->argv[0];
	paths = get_paths(data->envp_lst);
	if (is_absolute_path(statement))
	{
		if (execve(bin_path, statement->argv, data->envp) == -1)
			exit_and_free_matrix(paths, statement->argv[0], 127);
		exit_and_free_matrix(paths, NULL, EXIT_SUCCESS);
		return ;
	}
	bin_path = get_bin_path(statement->argv[0], paths);
	if (bin_path == NOT_FOUND)
	{
		cmd_not_found(statement->argv[0]);
		exit_and_free_matrix(paths, NULL, 127);
	}
	if (execve(bin_path, statement->argv, data->envp) == -1)
	{
		free(bin_path);
		exit_and_free_matrix(paths, statement->argv[0], 127);
	}
	free(bin_path);
	exit_and_free_matrix(paths, NULL, EXIT_SUCCESS);
} */