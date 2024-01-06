/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysantos- <ysantos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 15:35:02 by diosanto          #+#    #+#             */
/*   Updated: 2023/12/29 17:11:15 by ysantos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern long long	g_exit_status;

t_data	*ft_data(void)
{
	static t_data	data;

	return (&data);
}

/* Print a new line, clear the input line, update the display
and set the exit status to 130

1. Print a new line to the standard output
2. Notify read line that a new line has started
3. Replace the current line in Readline buffer with an empty string
4. Redisplay the Readline prompt and input buffer
5. Set the global exit status to 130 */
void	dismiss_signal(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
	if (signum == SIGQUIT)
	{
		ft_putstr_fd("minishell: warning: unexpected EOF while looking for matching delimiter\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
}

/* Setup a custom signal handler for SIGINT, configures the restart flag,
and ignores the SIGQUIT signal.

1. Set the signal handler function to dismiss signal
2. Set the flags to SA_RESTART to restart uninterrupted system calls 
3. Clean the signal mask in sa
4. Add SIGINT to the signal mask in sa
5. Set the action for SIGINT using the sa configuration(when SIGINT is called
the dismiss signal function will be called)
6. Ignore the SIGQUIT signal*/
void	config_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &dismiss_signal;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
