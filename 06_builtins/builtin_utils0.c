/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 15:01:07 by musenov           #+#    #+#             */
/*   Updated: 2023/11/27 18:01:06 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

////////////////////////////// cd //////////////////////////////

char	*get_envp_ll_var_value(t_envp_ll *head, char *var)
{
	t_envp_ll	*temp;

	temp = head;
	while (temp)
	{
		if (ft_strncmp(temp->var, var, ft_strlen(var) + 1) == 0)
			return (ft_strdup(temp->value));
		temp = temp->next;
	}
	print_error_bltn("cd", "-", "OLDPWD not set");
	return (NULL);
}

int	update_envp_ll_var_value(t_envp_ll *head, char *var, char *value)
{
	t_envp_ll	*temp;

	temp = head;
	while (temp)
	{
		if (ft_strncmp(temp->var, var, ft_strlen(var) + 1) == 0)
		{
			if (temp->value)
				free(temp->value);
			temp->value = ft_strdup(value);
			if (temp->value == NULL)
				return (EXIT_FAILURE);
			break ;
		}
		temp = temp->next;
	}
	if (!temp)
	{
		// if (add_to_var_list(head, var, 1))
		// 	return (EXIT_FAILURE);
		// update_envp_ll_var_value(head, var, value);
		add_to_list(&head, var, value);
	}
	return (EXIT_SUCCESS);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (s1 == 0 || s2 == 0)
		return (0);
	while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (0);
	return (1);
}

////////////////////////////// echo //////////////////////////////

void	print_internal_error(char *msg)
{
	ft_putstr_fd("minishell internal error: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

////////////////////////////// env //////////////////////////////

//								-//-

////////////////////////////// exit //////////////////////////////

//								-//-
