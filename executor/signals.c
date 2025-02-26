/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:36:43 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 15:00:28 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void signal_handler(int signum);
static void signal_ignore_handler(int signum);

void setup_interactive_signals(void)
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
}

static void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        g_last_status = 1;
        printf("\n");
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
    else if (signum == SIGTERM)
        exit(0);
}

void ignore_signals(void)
{
    signal(SIGINT, signal_ignore_handler);
    signal(SIGTERM, signal_ignore_handler);
}

static void signal_ignore_handler(int signum)
{
    (void)signum;
    return;
}

void handle_exec_signals(int signum)
{
    if (signum == SIGINT)
        g_last_status = 130;
    if (signum == SIGQUIT)
        g_last_status = 131;
}
