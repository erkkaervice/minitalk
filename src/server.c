/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:49:20 by eala-lah          #+#    #+#             */
/*   Updated: 2024/12/17 14:37:54 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
 * g_pid - Tracks the current client process communicating with the server.
 *
 * This volatile variable is set to the PID of the client that initiates
 * communication. It prevents the server from processing signals from
 * multiple clients simultaneously.
 */
volatile sig_atomic_t	g_pid = 0;

/*
 * ft_buffer - Manages dynamic message buffering.
 *
 * This function appends a character to a dynamic buffer. If the buffer is
 * full, it reallocates memory to double its size. The function maintains the
 * current index and buffer size for efficient storage.
 *
 * Parameters:
 * - chr: The character to append to the buffer.
 * - ind: Pointer to the current index in the buffer.
 * - bsize: Pointer to the current buffer size.
 * - buf: Double pointer to the buffer.
 */
void	ft_buffer(int chr, int *ind, int *bsize, char **buf)
{
	int		i;
	int		pre;
	char	*new;

	if (*ind >= *bsize)
	{
		new = malloc(*bsize * 2);
		if (!new)
			ft_error("MEMORY ALLOCATION FAILED\n");
		i = 0;
		pre = *ind;
		while (i < pre)
		{
			new[i] = (*buf)[i];
			i++;
		}
		free(*buf);
		*buf = new;
		*bsize *= 2;
	}
	(*buf)[*ind] = chr;
	(*ind)++;
}

/*
 * ft_endmsg - Finalizes and prints the received message.
 *
 * Once the null character is received, this function terminates the message,
 * writes it to the standard output, and resets the buffer for the next
 * message. It also acknowledges the client.
 *
 * Parameters:
 * - buf: Double pointer to the buffer containing the message.
 * - ind: Pointer to the current index in the buffer.
 * - bsize: Pointer to the buffer size.
 * - pid: The PID of the client to acknowledge.
 */
static void	ft_endmsg(char **buf, int *ind, int *bsize, pid_t pid)
{
	if (*buf)
	{
		(*buf)[*ind] = '\0';
		write(1, *buf, *ind);
		write(1, "\n", 1);
		free(*buf);
	}
	*buf = NULL;
	*ind = 0;
	*bsize = 128;
	ft_signal(pid, SIGUSR1);
	g_pid = 0;
}

/*
 * ft_process - Handles a character by appending it to the buffer or finalizing
 * the message.
 *
 * If the character is not null, it is added to the buffer. If it is null,
 * the message is finalized and printed. The function also manages memory for
 * the buffer as needed.
 *
 * Parameters:
 * - chr: The character to process.
 * - pid: The PID of the client to acknowledge.
 */
void	ft_process(int chr, pid_t pid)
{
	static char	*buf = NULL;
	static int	ind = 0;
	static int	bsize = 128;

	if (!buf)
	{
		buf = malloc(bsize);
		if (!buf)
			ft_error("MEMORY ALLOCATION FAILED\n");
	}
	if (chr == '\0')
		ft_endmsg(&buf, &ind, &bsize, pid);
	else
		ft_buffer(chr, &ind, &bsize, &buf);
}

/*
 * ft_receive - Signal handler for receiving and assembling characters.
 *
 * This function interprets incoming signals as bits, constructs characters,
 * and processes them. It maintains synchronization by acknowledging each
 * received bit to the client.
 *
 * Parameters:
 * - sig: The received signal (SIGUSR1 or SIGUSR2).
 * - info: Signal metadata, including the sender's PID.
 * - birds: Unused parameter (birds aren't real).
 */
void	ft_receive(int sig, siginfo_t *info, void *birds)
{
	static int		chr = 0;
	static int		bit = 7;
	static pid_t	pid = 0;

	(void)birds;
	if (g_pid == 0)
	{
		pid = info->si_pid;
		g_pid = pid;
	}
	else if (info->si_pid != pid)
		return ;
	ft_bit(sig, &chr, &bit);
	if (bit < 0)
	{
		ft_process(chr, pid);
		chr = 0;
		bit = 7;
	}
	if (kill(pid, SIGUSR1) == -1)
		ft_error("PROBLEM WITH SIGNAL, TRY TELEGRAM\n");
}

/*
 * main - Initializes the server and handles incoming messages.
 *
 * The function displays the server's PID, sets up signal handlers, and enters
 * an infinite loop to process incoming signals. Signals are handled by
 * `ft_receive`, which assembles and processes the message.
 *
 * Returns:
 * - 0 on successful execution (never exits as the server runs indefinitely).
 */
int	main(void)
{
	struct sigaction	sa;

	ft_printf("PID: %d\n", getpid());
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = ft_receive;
	if ((sigaction(SIGUSR1, &sa, NULL) == -1)
		|| (sigaction(SIGUSR2, &sa, NULL) == -1))
		ft_error("NOT GETTING IT\n");
	while (1)
		pause();
	return (0);
}
