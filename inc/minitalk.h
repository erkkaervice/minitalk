/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:49:05 by eala-lah          #+#    #+#             */
/*   Updated: 2024/12/05 14:45:22 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

/* Includes necessary libraries for the project.
 * - libft: Provides utility functions for string manipulation,
 *   memory handling, and other common tasks.
 * - signal: Provides functions for signal handling to enable
 *   inter-process communication.
 */
# include "../libft/inc/libft.h"
# include <signal.h>

/* --- Constants ---
 * Constants related to retry logic and signal handling.
 * - MAX_RETRY: Defines the maximum number of retry attempts for
 *   signal transmission to ensure message delivery.
 * - RETRY_DELAY: Specifies the delay in microseconds between
 *   retry attempts to avoid overloading the system.
 */
# define MAX_RETRY 5
# define RETRY_DELAY 1000000

/* --- Signal Handling Functions ---
 * Functions related to sending signals for inter-process communication.
 * - ft_signal: Sends a signal to a specific process to communicate
 *   data or control information.
 */
void	ft_signal(int pid, int signal);

/* --- Bit Manipulation Functions ---
 * Functions responsible for transmitting and processing bits.
 * - ft_bit: Handles the transmission of bits over signals, converting
 *   characters into individual bits for transfer.
 */
void	ft_bit(int sig, int *chr, int *bit);

#endif
