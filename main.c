/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 20:46:03 by musenov           #+#    #+#             */
/*   Updated: 2023/10/03 16:59:11 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// extern char	**environ;

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_minishell	shell_data;
	int			i;
	t_pipe		data;

	(void)argc;
	(void)argv;
	(void)envp;
	init_pipe_data(&data, &shell_data);
	init_shell(&shell_data);
	shell_data.data = &data;
	while (1)
	{
		ms_terminal_settings_change();
		set_signals_interactive();
		// printf("Exit code0: %d\n", data.exit_code);
		line = readline(RED "minishell> " RESET);
		ms_terminal_settings_restore();
		set_signals_noninteractive();
		if (line != NULL)
		{
			if (line[0] != '\0')
			{
				add_history(line);
				if (!tokenize(&shell_data, line))
				{
					shell_data.ast_root = parse_pipeline(&shell_data);
					if (shell_data.ast_root)
					{
						free_tokens(&shell_data);
						// print_ast_tree0(shell_data.ast_root, 0);
						i = 0;
						if (shell_data.ast_root->type == AST_NODE_CMD)
						{
							piper(&data, &i);
							forker_no_pipe(&data, shell_data.envp_local, shell_data.ast_root);
							free_ast(&shell_data.ast_root);
						}
						else
						{
							data.nr_of_cmd_nodes = 0;
							execute_cmds(shell_data.ast_root, &i, &data, shell_data.envp_local);
							free_ast(&shell_data.ast_root);
						}
					}
					else
					{
						// printf("PARSER FAILED\n");
						free_tokens(&shell_data);
						free_ast(&shell_data.ast_root);
					}
				}
				else
				{
					// printf("LEXER FAILED\n");
					free_tokens(&shell_data);
				}
				free(line);
				ft_waiting(&data);
				// printf("Exit code1: %d\n", data.exit_code);
			}
		}
		else
		{
			exit_for_signals(&data);
			// printf("Ctrl + D pressed, exit code is %d", data.exit_code);
			break ;
		}
		exit_for_signals(&data);
		// printf("Exit code2: %d\n", data.exit_code);
	}
	free_envp_ll(shell_data.envp_ll);
	free_envp_local(shell_data.envp_local);
	return (data.exit_code);
}
