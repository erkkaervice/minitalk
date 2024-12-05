/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:19:00 by eala-lah          #+#    #+#             */
/*   Updated: 2024/12/05 14:19:58 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
 * ft_bit - Updates the character's value based on the signal bit received.
 *
 * This function modifies the character being constructed by updating 
 * its specific bit based on the signal received. SIGUSR1 sets the bit, 
 * while SIGUSR2 clears it. After processing the bit, the function decrements 
 * the bit index to move to the next position for subsequent signals.
 *
 * Parameters:
 * - sig: The signal received, determining whether to set or clear a bit 
 *        (SIGUSR1 sets, SIGUSR2 clears).
 * - chr: Pointer to the character being updated, modified bit by bit.
 * - bit: Pointer to the current bit index being processed, starting at 7.
 */
void	ft_bit(int sig, int *chr, int *bit)
{
	if (sig == SIGUSR1)
		*chr |= (1 << *bit);
	else if (sig == SIGUSR2)
		*chr &= ~(1 << *bit);
	(*bit)--;
}

/*
 * ft_signal - Sends a specified signal to a process.
 *
 * This function transmits a signal (SIGUSR1 or SIGUSR2) to a target process 
 * identified by its process ID (PID). It ensures reliable signal delivery 
 * and terminates the program with an error message if the signal cannot 
 * be sent.
 *
 * Parameters:
 * - pid: The process ID of the target process.
 * - signal: The signal to be sent (either SIGUSR1 or SIGUSR2).
 */
void	ft_signal(int pid, int signal)
{
	if (kill(pid, signal) == -1)
		ft_error("PROBLEM WITH SIGNAL, TRY TELEGRAM\n");
}
