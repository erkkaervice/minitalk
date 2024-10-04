/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:49:08 by eala-lah          #+#    #+#             */
/*   Updated: 2024/09/16 12:37:38 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

# include "../libft/inc/libft.h"
# include <signal.h>

# define MAX_RETRY 5
# define RETRY_DELAY 1000000

void	ft_signal(int pid, int signal);
void	ft_bit(int sig, int *chr, int *bit);

#endif