/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:49:20 by eala-lah          #+#    #+#             */
/*   Updated: 2024/09/16 12:31:43 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_pid = 0;

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
