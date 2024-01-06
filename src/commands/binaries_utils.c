/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diosanto <diosanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 19:03:25 by diosanto          #+#    #+#             */
/*   Updated: 2024/01/06 19:05:00 by diosanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_absolute_path(t_statement *statement)
{
	if (is_onstr(statement->argv[0], '/'))
		return (true);
	return (false);
}
