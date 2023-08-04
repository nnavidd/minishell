/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 13:21:08 by musenov           #+#    #+#             */
/*   Updated: 2023/08/04 19:45:13 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_cmd(t_pipe *data, char **envp)
{
	if (data->pid == 0)
	{
		find_cmd_path(data, envp);
		dup2(data->pipe0_fd[1], STDOUT_FILENO);
		close_pipe0_fds(data);
		execve(data->cmd_path, data->cmd_split, envp);
	}
}

void	middle_cmd(t_pipe *data, char **envp, int *i)
{
	if (data->pid == 0)
	{
		find_cmd_path(data, envp);
		if (*i % 2 == 0)
		{
			dup2(data->pipe1_fd[0], STDIN_FILENO);
			dup2(data->pipe0_fd[1], STDOUT_FILENO);
		}
		else
		{
			dup2(data->pipe0_fd[0], STDIN_FILENO);
			dup2(data->pipe1_fd[1], STDOUT_FILENO);
		}
		close_pipe0_fds(data);
		close_pipe1_fds(data);
		execve(data->cmd_path, data->cmd_split, envp);
	}
	if (*i % 2 == 0)
		close_pipe1_fds(data);
	else
		close_pipe0_fds(data);
}

void	last_cmd(t_pipe *data, char **envp, int *i)
{
	if (data->pid == 0)
	{
		find_cmd_path(data, envp);
		if (*i % 2 == 0)
		{
			dup2(data->pipe1_fd[0], STDIN_FILENO);
		}
		else
		{
			dup2(data->pipe0_fd[0], STDIN_FILENO);
		}
		close_pipe0_fds(data);
		close_pipe1_fds(data);
		execve(data->cmd_path, data->cmd_split, envp);
	}
	close_pipe0_fds(data);
	close_pipe1_fds(data);
}