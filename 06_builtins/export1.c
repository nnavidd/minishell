/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:12:11 by musenov           #+#    #+#             */
/*   Updated: 2023/11/17 20:41:17 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* is_valid_env_var_key:
*	Checks if the key is a valid name for an evironment
*	variable.
*	Returns true if the key contains only alphanumeric chars
*	or '_', or false if not.
*/
bool	valid_identifier(char *var)
{
	int	i;

	i = 0;
	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
		return (false);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	valid_identifier_unset(char *var)
{
	int	i;

	i = 0;
	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
		return (false);
	i++;
	while (var[i])
	// while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

t_envp_ll	*find_var_node(t_envp_ll *head, char *cmd)
{
	t_envp_ll	*current;

	current = head;
	while (current != NULL)
	{
		if (ft_strncmp(current->var, cmd, ft_strlen(cmd) + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	**make_var_value_pair(char *cmd)
{
	char	*equal_sign;
	char	**var_value_pair;

	equal_sign = ft_strchr(cmd, '=');
	var_value_pair = (char **)malloc(sizeof(char *) * 3);
	var_value_pair[0] = ft_substr(cmd, 0, equal_sign - cmd);
	var_value_pair[1] = ft_substr(equal_sign, 1, ft_strlen(equal_sign));
	var_value_pair[2] = NULL;
	return (var_value_pair);
}

int	export_var(t_envp_ll *head, char *cmd)
{
	t_envp_ll	*var_node;
	char		**var_value_pair;

	var_value_pair = make_var_value_pair(cmd);
	var_node = find_var_node(head, var_value_pair[0]);
	if (var_node)
	{
		free(var_node->value);
		if (var_value_pair[1])
		{
			var_node->value = ft_strdup(var_value_pair[1]);
			if (!var_node->value)
			{
				free_2d_str_func(var_value_pair);
				return (EXIT_FAILURE);
			}
		}
		else
			var_node->value = ft_strdup("");
		var_node->env_var = true;
	}
	else if (var_value_pair[1])
		add_to_list(&head, var_value_pair[0], var_value_pair[1]);
	else
		add_to_list(&head, var_value_pair[0], "");
	free_2d_str_func(var_value_pair);
	return (EXIT_SUCCESS);
}

void	export_no_args(t_envp_ll *var_head)
{
	t_envp_ll	*current;

	current = var_head;
	while (current != NULL)
	{
		if (current->env_var == true)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(current->var, STDOUT_FILENO);
			if (current->value != NULL)
			{
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(current->value, STDOUT_FILENO);
				ft_putchar_fd('\"', STDOUT_FILENO);
			}
			ft_putchar_fd('\n', STDERR_FILENO);
		}
		current = current->next;
	}
	return ;
}

/* export_builtin:
*	Adds the given variables to the environment variables.
*	Returns 0 if all args were successfully added to env,
*	or 1 if one or more args were not added to env.
*/
int	export_bltn(t_envp_ll *head, char **cmd, t_minishell *shell)
{
	int		i;
	int		return_value;

	return_value = 0;
	i = 1;
	if (!cmd[i])
	{
		export_no_args(head);
		return (return_value);
	}
	while (cmd[i])
	{
		if (valid_identifier(cmd[i]))
		{
			if (ft_strchr(cmd[i], '='))
				export_var(head, cmd[i]);
		}
		else
		{
			if (cmd[1][0] == '-')
			{
				print_error_bltn("export", cmd[i], "options are not required");
				return_value = 2;
			}
			else
			{
				print_error_bltn("export", cmd[i], "not a valid identifier");
				return_value = 1;
			}
		}
		i++;
	}
	free_envp_local(shell->envp_local);
	envp_ll_to_envp_local(shell);
	return (return_value);
}
