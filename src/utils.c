/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:19:00 by eala-lah          #+#    #+#             */
/*   Updated: 2024/09/13 13:11:05 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_bit(int sig, int *chr, int *bit)
{
	if (sig == SIGUSR1)
		*chr |= (1 << *bit);
	else if (sig == SIGUSR2)
		*chr &= ~(1 << *bit);
	(*bit)--;
}

void	ft_signal(int pid, int signal)
{
	if (kill(pid, signal) == -1)
		ft_error("PROBLEM WITH SIGNAL, TRY TELEGRAM\n");
}
