/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_local_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 12:33:16 by musenov           #+#    #+#             */
/*   Updated: 2023/09/28 15:36:25 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Main function to transform linked list to 2D array
// char	**envp_ll_to_envp_local(t_envp_ll *head)
void	envp_ll_to_envp_local(t_minishell *shell)
{
	int			count;
	char		**array;
	int			i;
	t_envp_ll	*head;

	head = shell->envp_ll;
	count = count_nodes(head);
	array = malloc((count + 1) * sizeof(char *));
	if (!array)
		return ;
	i = 0;
	while (head)
	{
		array[i] = allocate_and_set_entry(head);
		if (!array[i])
		{
			// while (i--)
			// 	free(array[i]);
			// free(array);
			free_envp_local(array);
			return ;
		}
		head = head->next;
		i++;
	}
	array[count] = NULL;
	shell->envp_local = array;
}

// Function to count nodes in the linked list
int	count_nodes(t_envp_ll *head)
{
	int		count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	*allocate_and_set_entry(t_envp_ll *node)
{
	// char *equal_sign = "=";
	char	*key_and_equal_sign;
	char	*entry;

	key_and_equal_sign = ft_strjoin(node->var, "=");
	if (!key_and_equal_sign)
		return (NULL);
	entry = ft_strjoin(key_and_equal_sign, node->value);
	free(key_and_equal_sign);
	return (entry);
}
