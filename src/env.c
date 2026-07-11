#include "../include/minishell.h"

t_env *new_env_node(char *key, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

void add_env_back(t_env **env, t_env *new)
{
    t_env *tmp;

    if (!*env)
    {
        *env = new;
        return;
    }
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

t_env *init_env(char **envp)
{
    t_env *env = NULL;
    int     i = 0;
    char   *eq;
    char   *key;
    char   *value;

    while (envp[i])
    {
        eq = ft_strchr(envp[i], '=');
        if (!eq)
        {
            i++;
            continue;
        }

        key = ft_substr(envp[i], 0, eq - envp[i]);
        value = ft_strdup(eq + 1);

        add_env_back(&env, new_env_node(key, value));
        i++;
    }
    return env;
}
