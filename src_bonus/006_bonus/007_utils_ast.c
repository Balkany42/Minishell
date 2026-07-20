/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   007_utils_ast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 08:31:57 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 08:32:00 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

char	*ast_to_string(t_ast *node)
{
	char	*l;
	char	*r;
	char	*res;

	if (!node)
		return (ft_strdup(""));
	if (node->op == AST_NONE)
		return (cmd_to_string(node->cmd));
	if (node->op == AST_GROUP)
		return (ast_to_string(node->left));
	if (node->op == AST_AND_IF)
	{
		l = ast_to_string(node->left);
		r = ast_to_string(node->right);
		res = ft_strjoin3(l, " && ", r);
		return (free(l), free(r), res);
	}
	if (node->op == AST_OR_IF)
	{
		l = ast_to_string(node->left);
		r = ast_to_string(node->right);
		res = ft_strjoin3(l, " || ", r);
		return (free(l), free(r), res);
	}
	return (ft_strdup(""));
}

char	*cmd_to_string(t_cmd *cmd)
{
	char	*res;
	char	*tmp;
	int		i;
	char	*sep;

	res = ft_strdup("");
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (i == 0)
			sep = "";
		else
			sep = " ";
		tmp = ft_strjoin3(res, sep, cmd->args[i]);
		free(res);
		res = tmp;
		i++;
	}
	return (res);
}

int	is_redir(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND
		|| type == HEREDOC);
}

char	*ft_strjoin3(const char *a, const char *b, const char *c)
{
	size_t	la;
	size_t	lb;
	size_t	lc;
	char	*res;

	la = ft_strlen(a);
	lb = ft_strlen(b);
	lc = ft_strlen(c);
	res = malloc(la + lb + lc + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, a, la);
	ft_memcpy(res + la, b, lb);
	ft_memcpy(res + la + lb, c, lc);
	res[la + lb + lc] = '\0';
	return (res);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}
