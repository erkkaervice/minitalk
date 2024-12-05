/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:49:33 by eala-lah          #+#    #+#             */
/*   Updated: 2024/12/05 14:18:27 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
 * g_ack - Global acknowledgment flag for client-server communication.
 *
 * This volatile variable is set to 1 when the server sends a SIGUSR1 signal,
 * indicating successful receipt of a bit. It allows the client to synchronize
 * communication by waiting for acknowledgment before sending the next bit.
 */
volatile sig_atomic_t	g_ack;

/*
 * ft_validate - Ensures the validity of the server's PID.
 *
 * This function checks if the provided PID is positive and corresponds to
 * an active process. If the PID is invalid or inactive, the program exits
 * with an error message.
 *
 * Parameters:
 * - pid: The process ID of the server to validate.
 */
void	ft_validate(pid_t pid)
{
	if (pid <= 0)
		ft_error("THINK POSITIVE!\n");
	if (kill(pid, 0) == -1)
		ft_error("NOT A VALID PID...\n");
}

/*
 * ft_acker - Signal handler for acknowledgment signals.
 *
 * This function sets the global acknowledgment flag (`g_ack`) to 1 when the
 * server sends a SIGUSR1 signal, allowing the client to proceed.
 *
 * Parameters:
 * - sig: The received signal (expected to be SIGUSR1).
 */
void	ft_acker(int sig)
{
	if (sig == SIGUSR1)
		g_ack = 1;
}

/*
 * ft_bits - Sends a single bit to the server with acknowledgment handling.
 *
 * This function transmits a bit (1 or 0) to the server using SIGUSR1 or
 * SIGUSR2 signals. It waits for acknowledgment within a retry loop,
 * ensuring reliable transmission even in case of communication delays.
 *
 * Parameters:
 * - pid: The server's process ID.
 * - bit: The bit to transmit (0 or 1).
 */
void	ft_bits(int pid, int bit)
{
	int	retry;

	retry = 0;
	while (retry < MAX_RETRY)
	{
		if (bit)
			ft_signal(pid, SIGUSR1);
		else
			ft_signal(pid, SIGUSR2);
		usleep(100);
		while (!g_ack && retry < MAX_RETRY)
		{
			usleep(RETRY_DELAY);
			retry++;
		}
		if (g_ack)
		{
			g_ack = 0;
			return ;
		}
	}
	ft_error("SERVER IS BUSY, TRY AGAIN LATER\n");
}

/*
 * ft_send - Transmits a character to the server bit by bit.
 *
 * Each bit of the character is sent using the `ft_bits` function. If the
 * transmitted character is a null terminator (`\0`) and the `end` flag is set,
 * a success message is displayed.
 *
 * Parameters:
 * - pid: The server's process ID.
 * - c: The character to transmit.
 * - end: Flag indicating whether the character is the last in the message.
 */
void	ft_send(int pid, char c, int end)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		ft_bits(pid, (c >> bit) & 1);
		bit--;
	}
	if (end && !c)
		ft_printf("VERY SUCCESS!\n");
}

/*
 * main - Entry point of the client program.
 *
 * The function validates input arguments, sets up the signal handler, and
 * transmits the message to the server character by character. It sends a
 * null character to signify the end of the message.
 *
 * Parameters:
 * - argc: The number of command-line arguments.
 * - argv: Array of arguments, including the server PID and message.
 *
 * Returns:
 * - 0 on successful execution.
 */
int	main(int argc, char **argv)
{
	pid_t				pid;
	char				*msg;
	struct sigaction	sa;

	if (argc != 3 || !(*argv[2]))
		ft_error("TRY: ./client <pid> <message>\n");
	pid = ft_atoi(argv[1]);
	ft_validate(pid);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_handler = ft_acker;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		ft_error("NOT GIVING IT\n");
	msg = argv[2];
	while (*msg)
		ft_send(pid, *msg++, 0);
	ft_send(pid, '\0', 1);
	return (0);
}
