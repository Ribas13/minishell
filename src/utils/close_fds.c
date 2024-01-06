/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 20:43:41 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 20:43:51 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Close all fds for child process */
void	close_all_fds(int pipe[2])
{
	if (pipe)
	{
		close(pipe[0]);
		close(pipe[1]);
	}
	if (ft_data()->default_stdin > 2)
		close(ft_data()->default_stdin);
	if (ft_data()->default_stdout > 2)
		close(ft_data()->default_stdout);
}
