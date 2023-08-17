/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 23:17:32 by musenov           #+#    #+#             */
/*   Updated: 2023/08/18 00:06:26 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_ast_node *create_command_node(char **cmd) {
//     t_ast_node *node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
//     if (!node) {
//         perror("Memory allocation error");
//         exit(1);
//     }
//     node->type = AST_NODE_CMD;
//     node->content = (t_ast_node_content *)ft_calloc(1, sizeof(t_ast_node_content));
//     if (!node->content) {
//         perror("Memory allocation error");
//         exit(1);
//     }
//     node->content->cmd = cmd;
//     node->left = NULL;
//     node->right = NULL;
//     return node;
// }

t_ast_node *create_command_node(t_ast_node_content *content)
{
	t_ast_node *node;

	node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		perror("Memory allocation error"); //error handeling
		exit(1);
	}
	node->type = AST_NODE_CMD;
	node->content = content;
	node->left = NULL;
	node->right = NULL;
	return node;
}

t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node;

	node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		perror("Memory allocation error"); //error handling
		exit(1);
	}
	node->type = AST_NODE_PIPE;
	node->content = NULL;
	node->left = left;
	node->right = right;
	return node;
}

t_ast_node_content *parse_command_content(t_ast_node_content **content, t_token **tokens, int *token_count)
{
	int	current;
	int cmd_index;

	current = *token_count;
	cmd_index = 0;
	if (*token_count >= 0 && (*tokens)[*token_count - 1].type == TOKEN_PIPE)
	{
		free(*content);
		*content = NULL;
		return NULL;
	}
	while (current-- > 0 && (*tokens)[current].type != TOKEN_PIPE) //since token_count is one more than itrator
		cmd_index++;
	(*content)->cmd = (char **)ft_calloc((cmd_index + 1), sizeof(char *));
	(*content)->cmd[cmd_index] = NULL;
	while (cmd_index-- > 0)
	{
		if ((*tokens)[*token_count - 1].value != NULL)
		{
			(*content)->cmd[cmd_index] = ft_strdup((*tokens)[*token_count - 1].value);
			free((*tokens)[*token_count - 1].value);
			(*tokens)[*token_count - 1].value = NULL;
		}
		else
			(*content)->cmd[cmd_index] = NULL;
		(*token_count)--;
	}
	return (*content);
}

t_ast_node *parse_command(t_token **tokens, int *token_count)
{
	t_ast_node_content *content;

	content = (t_ast_node_content *)ft_calloc(1, sizeof(t_ast_node_content));
	if (!content)
	{
		perror("Memory allocation error"); //error handling
		exit(1);
	}
	content->stdin_redirect = NULL;
	content->stdout_redirect = NULL;
	content->assignments = NULL;
	content->cmd = NULL;
	parse_command_content(&content, tokens, token_count);
	if (content == NULL)
		return NULL;  // Return NULL if command content is empty (due to PIPE)
	return create_command_node(content);
}

t_ast_node *parse_pipeline(t_token **tokens, int *token_count)
{
	t_ast_node *left;
	t_ast_node *right;

	right = parse_command(tokens, token_count);

	if (*token_count > 0 && (*tokens)[*token_count - 1].type == TOKEN_PIPE)
	{
		free((*tokens)[*token_count - 1].value);
		(*tokens)[*token_count - 1].value = NULL;
		(*token_count)--;  // Consume the pipe operator
		left = parse_pipeline(tokens, token_count);
		return create_pipe_node(left, right);
	}
	return right;
}

void free_ast(t_ast_node **node_ptr)
{
	if (!(*node_ptr))
		return;

	if ((*node_ptr)->type == AST_NODE_CMD) {
		if ((*node_ptr)->content) {
			if ((*node_ptr)->content->cmd) {
				int i = 0;
				while ((*node_ptr)->content->cmd[i] != NULL) {
					free((*node_ptr)->content->cmd[i]);
					(*node_ptr)->content->cmd[i] = NULL;
					i++;
				}
				free((*node_ptr)->content->cmd);
				(*node_ptr)->content->cmd = NULL;
			}
			free((*node_ptr)->content);
			(*node_ptr)->content = NULL;
		}
	} else if ((*node_ptr)->type == AST_NODE_PIPE) {
		free_ast(&((*node_ptr)->left));
		free_ast(&((*node_ptr)->right));
		(*node_ptr)->left = NULL;
		(*node_ptr)->right = NULL;
	}

	free(*node_ptr);
	*node_ptr = NULL;
}

void print_ast_node(t_ast_node *node, int level, char x)
{
	if (x == 'x')
		printf("\n***************** AST ****************\n");
	if (node == NULL) {
		return;
	}
	for (int i = 0; i < level; i++)
		printf("  ");
	if (x == 'l')
		printf(BLUE "Left child:\n");
	if (x == 'r')
		printf(BLUE "Right child:\n" RESET);
	for (int i = 0; i < level; i++)
		printf("    ");
	if (node->type == AST_NODE_CMD) {
		printf(RED "Node type:" RESET ORG" AST_NODE_CMD\n" RESET);
		if (node->content)
		{
			for (int i = 0; i < level; i++)
				printf("    ");
			printf(RED "Content:\n"RESET);
			if (node->content->cmd)
			{
				for (int i = 0; i < level; i++)
					printf("    ");
				printf("Command:");
				for (int i = 0; node->content->cmd[i] != NULL; i++) {
					printf(ORG " %s" RESET, node->content->cmd[i]);
				}
				printf("\n");
			}
		}
	} else if (node->type == AST_NODE_PIPE) {
		printf(RED "Node type:"RESET ORG" AST_NODE_PIPE\n" RESET);
	}

	print_ast_node(node->left, level + 1, 'l');
	print_ast_node(node->right, level + 1, 'r');
}
