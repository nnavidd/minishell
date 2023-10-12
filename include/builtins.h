/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 14:38:02 by musenov           #+#    #+#             */
/*   Updated: 2023/10/12 19:08:55 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "common.h"

// builtin_calls.c

bool			is_builtin(char *cmd);
int				execute_bltn(t_minishell *shell, char **cmd);

// echo.c

int				echo_bltn(char **cmd);
int				check_option_n(char **arg);
int				builtin_echo_strjoin(char **cmd, int cnt, int option_n);

// export.c

int				export_bltn(t_envp_ll *var_head, char **cmd);
void			export_no_args(t_envp_ll *var_head);
int				is_valid_identifier(char *cmd);
int				export_one(t_envp_ll *var_head, char *cmd);

// unset.c

int				unset_is_valid_identifier(char *cmd);
t_envp_ll		*get_previous(t_envp_ll *var_head, t_envp_ll *found_node);
void			free_single_var_node(t_envp_ll *node);
void			remove_found_node(t_envp_ll *var_head, t_envp_ll *found_node);
int				unset_bltn(t_envp_ll *var_head, char **cmd);

// env.c

int				env_bltn(t_envp_ll *var_list, char **cmd);

// pwd.c

int				pwd_bltn(void);

// cd.c

int				cd_to_oldpwd(t_envp_ll *var_head, char *pwd);
int				cd_to_home(t_envp_ll *var_head, char *pwd);
int				cd_with_path(t_envp_ll *var_head, char **cmd, char *pwd);
int				execute_cd(t_envp_ll *var_head, char **cmd);
int				cd_bltn(t_envp_ll *var_head, char **cmd);


// builtin_utils.c

// echo
char			*ft_strjoin_sym(const char *s1, const char *s2, char c);
void			free_p(char	*p);
void			internal_error_printer(char *msg);
// export
int				get_arg_count(char **cmd);
void			error_printer(char *cmd, char *arg, char *msg);
t_envp_ll		*get_var_list_node(t_envp_ll *var_head, char *str);
int				modify_var_list_node(t_envp_ll *matching_node, char *cmd);
int				add_to_var_list(t_envp_ll *var_list, char *str, int flag);
int				ft_sub_len(char const *start, char c);
int				full_assign(t_envp_ll *new, char *word, char *delim_pt);
void			ft_lstadd_back_ms(t_envp_ll **var_list, t_envp_ll *new);
char			*ft_strdup_pt(const char *start, char *delimiter);
// cd
char			*get_value_for_key(t_envp_ll *var_head, const char *key);
int				change_value_for_key(t_envp_ll *var_head, char *key, \
									char *new_value);
bool			check_if_in_var_list(t_envp_ll *var_head, char *key);
int				ft_strcmp(const char *s1, const char *s2);

#endif