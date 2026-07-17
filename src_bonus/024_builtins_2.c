/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   021_builtins_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 03:55:52 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 04:01:24 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/minishell.h"

int	builtin_echo(char **args, t_minishell *sh)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_flag_n(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	sh->exit_status = 0;
	return (0);
}

int	is_flag_n(char *s)
{
	int	i;

	i = 0;
	if (s[i] != '-')
		return (0);
	i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_pwd(char **args, t_minishell *sh)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		sh->exit_status = 1;
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	sh->exit_status = 0;
	return (0);
}

int	builtin_cd(char **args, t_minishell *sh)
{
	char	*path;

	if (!args[1])
		path = env_get(sh->env, "HOME");
	else
		path = args[1];
	if (!path)
	{
		printf("minishell: cd: HOME not set\n");
		sh->exit_status = 1;
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		sh->exit_status = 1;
		return (1);
	}
	sh->exit_status = 0;
	return (0);
}

int	builtin_exit(char **args, t_minishell *sh)
{
	long	code;

	printf("exit\n");
	if (!args[1])
	{
		sh->should_exit = 1;
		sh->exit_status = sh->exit_status;
		return (0);
	}
	if (args[2])
    {
        printf("minishell: exit: too many arguments\n");
        sh->exit_status = 1;
        return (1);
    }
	if (!ft_isnumber(args[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		sh->should_exit = 1;
		sh->exit_status = 2;
		return (0);
	}
	code = ft_atol(args[1]);
	sh->should_exit = 1;
	sh->exit_status = (unsigned char)code;
	return (0);
}
