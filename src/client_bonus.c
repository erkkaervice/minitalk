/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:49:43 by eala-lah          #+#    #+#             */
/*   Updated: 2024/09/16 12:31:04 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

volatile sig_atomic_t	g_ack;

void	ft_validate(pid_t pid)
{
	if (pid <= 0)
		ft_error("THINK POSITIVE!\n");
	if (kill(pid, 0) == -1)
		ft_error("NOT A VALID PID...\n");
}

void	ft_acker(int sig)
{
	if (sig == SIGUSR1)
		g_ack = 1;
}

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
