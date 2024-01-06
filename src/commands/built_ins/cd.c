/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 19:51:37 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/24 19:54:11 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static int	print_error_msg(char *path)
{
	char	*error_msg;

	error_msg = ft_strjoin("minishell: cd: ", path);
	perror(error_msg);
	free(error_msg);
	return (EXIT_FAILURE);
}

/* Updates the PWD value in the envp list variables.
Retrieves te current working directory and replaces the
previous value for PWD var */
static void	update_pwd(t_data *data)
{
	char	cwd[PATH_MAX];
	char	*updated_var;

	getcwd(cwd, PATH_MAX);
	updated_var = ft_strjoin("PWD=", cwd);
	save_user_vars(updated_var, &data->envp_lst, true);
	free(updated_var);
}

/* Updates the old working directory variable on the envp list */
static void	update_old_pwd(char *temp, t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", temp);
	save_user_vars(oldpwd, &data->envp_lst, true);
	free(oldpwd);
}

/* Change current working directory to the previously directory saved
on the OLDPWD var */
static int	cd_oldpwd(char *temp, t_data *data)
{
	char	*oldpwd;

	oldpwd = get_fromvlst("OLDPWD", &data->envp_lst);
	if (!oldpwd)
	{
		ft_putendl_fd(OLDPWD_NOT_SET, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (chdir(oldpwd) == SUCCESS)
	{
		ft_putendl_fd(oldpwd, STDOUT_FILENO);
		update_old_pwd(&temp[0], data);
		update_pwd(data);
		return (EXIT_SUCCESS);
	}
	return (print_error_msg(oldpwd));
}

/* Changes the current working directory(based on a provided path)
and updates the old pwd

1. Fetch curr working dir and save to update the oldpwd later
2. Check if NULL or HOME dir (if home, change dir)
3. Check if it's the last working dir('-')(if so change dir)
4. Change dir and update old pwd */
int	cmd_cd(char *path, t_data *data)
{
	char	temp[PATH_MAX];

	getcwd(temp, PATH_MAX);
	if (path == NULL || streq(path, "~"))
	{
		update_old_pwd(&temp[0], data);
		chdir(getenv("HOME"));
		update_pwd(data);
		return (EXIT_SUCCESS);
	}
	if (streq(path, "-"))
		return (cd_oldpwd(&temp[0], data));
	if (chdir(path) == SUCCESS)
	{
		update_old_pwd(&temp[0], data);
		update_pwd(data);
		return (EXIT_SUCCESS);
	}
	return (print_error_msg(path));
}
