#include "../include/minishell.h"

int is_valid_identifier(char *s)
{
    if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
        return 0;

    for (int i = 1; s[i] && s[i] != '='; i++)
        if (!ft_isalnum(s[i]) && s[i] != '_')
            return 0;

    return 1;
}

void env_set(t_env **env, char *key, char *value)
{
    t_env *e = *env;

    while (e)
    {
        if (!ft_strcmp(e->key, key))
        {
            free(e->value);
            e->value = value ? ft_strdup(value) : NULL;
            return;
        }
        e = e->next;
    }
    t_env *new = malloc(sizeof(t_env));
    new->key = ft_strdup(key);
    new->value = value ? ft_strdup(value) : NULL;
    new->next = *env;
    *env = new;
}

int builtin_export(t_cmd *cmd, t_minishell *sh)
{
    char *arg;
    char *eq;
    char *key;
    char *value;

    if (!cmd->args[1])
    {
        print_export_sorted(sh->env);
        sh->exit_status = 0;
        return 0;
    }

    for (int i = 1; cmd->args[i]; i++)
    {
        arg = cmd->args[i];

        // Trouver '=' si présent
        eq = ft_strchr(arg, '=');

        // Extraire la clé (avant '=' ou toute la chaîne)
        if (eq)
            key = ft_substr(arg, 0, eq - arg);
        else
            key = ft_strdup(arg);

        // Vérifier validité de la clé
        if (!is_valid_identifier(key))
        {
            printf("minishell: export: `%s`: not a valid identifier\n", arg);
            free(key);
            sh->exit_status = 1;
            continue;
        }

        // Si pas de '=', juste ajouter la clé vide
        if (!eq)
        {
            env_set(&sh->env, key, NULL);
            free(key);
            continue;
        }

        // Sinon extraire la valeur
        value = ft_strdup(eq + 1);

        env_set(&sh->env, key, value);

        free(key);
        free(value);
    }

    sh->exit_status = 0;
    return 0;
}

int builtin_unset(t_cmd *cmd, t_minishell *sh)
{
    t_env *cur;
    t_env *prev;
    char  *key;

    if (!cmd->args[1])
    {
        sh->exit_status = 0;
        return 0;
    }

    for (int i = 1; cmd->args[i]; i++)
    {
        key = cmd->args[i];

        if (!is_valid_identifier(key))
        {
            printf("minishell: unset: `%s`: not a valid identifier\n", key);
            sh->exit_status = 1;
            continue;
        }

        cur = sh->env;
        prev = NULL;

        while (cur)
        {
            if (!ft_strcmp(cur->key, key))
            {
                if (prev)
                    prev->next = cur->next;
                else
                    sh->env = cur->next;

                free(cur->key);
                free(cur->value);
                free(cur);
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    sh->exit_status = 0;
    return 0;
}
char *env_get(t_env *env, char *key)
{
    while (env)
    {
        if (!ft_strcmp(env->key, key))
            return env->value;
        env = env->next;
    }
    return NULL;
}

void sort_env(char **tab)
{
    int     i, j;
    char    *tmp;

    for (i = 0; tab[i]; i++)
    {
        for (j = i + 1; tab[j]; j++)
        {
            if (ft_strcmp(tab[i], tab[j]) > 0)
            {
                tmp = tab[i];
                tab[i] = tab[j];
                tab[j] = tmp;
            }
        }
    }
}

void print_export_sorted(t_env *env)
{
    char **tab = env_to_tab(env);
    int   i = 0;

    if (!tab)
        return;

    sort_env(tab);

    while (tab[i])
    {
        char *eq = ft_strchr(tab[i], '=');

        if (eq)
        {
            // declare -x KEY="VALUE"
            write(1, "declare -x ", 11);
            write(1, tab[i], eq - tab[i] + 1); // KEY=
            write(1, "\"", 1);
            write(1, eq + 1, ft_strlen(eq + 1)); // VALUE
            write(1, "\"\n", 2);
        }
        else
        {
            // declare -x KEY
            write(1, "declare -x ", 11);
            write(1, tab[i], ft_strlen(tab[i]));
            write(1, "\n", 1);
        }
        i++;
    }

    // free tab
    i = 0;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}
int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, ":") == 0 || ft_strcmp(cmd, "!") == 0 || ft_strcmp(cmd, ".") == 0)
        return (1);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}
int exec_builtin(t_cmd *cmd, t_minishell *sh)
{
    char **args = cmd->args;

    if (ft_strcmp(cmd->args[0], ":") == 0)
    {
        sh->exit_status = 0;
        return(1);
    }
    if (ft_strcmp(cmd->args[0], "!") == 0)
    {
        sh->exit_status = 1;
        return(1);
    }
    if (ft_strcmp(cmd->args[0], ".") == 0)
    {
        printf(".: filename argument required\n");
        sh->exit_status = 2;
        return(1);
    }
    if (ft_strcmp(args[0], "echo") == 0)
        return (builtin_echo(args, sh));
    if (ft_strcmp(args[0], "cd") == 0)
        return (builtin_cd(args, sh));
    if (ft_strcmp(args[0], "pwd") == 0)
        return (builtin_pwd(args, sh));
    if (ft_strcmp(args[0], "export") == 0)
        return (builtin_export(cmd, sh));
    if (ft_strcmp(args[0], "unset") == 0)
        return (builtin_unset(cmd, sh));
    if (ft_strcmp(args[0], "env") == 0)
        return (builtin_env(args, sh));
    if (ft_strcmp(args[0], "exit") == 0)
        return (builtin_exit(args, sh));

    return (0);
}
int builtin_echo(char **args, t_minishell *sh)
{
    int i = 1;
    int newline = 1;

    // Gestion correcte de -n, -nn, -nnnn...
    while (args[i] && is_flag_n(args[i]))
    {
        newline = 0;
        i++;
    }

    // Affichage des arguments restants
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

int is_flag_n(char *s)
{
    int i = 0;

    if (s[i] != '-')
        return 0;
    i++;
    if (s[i] == '\0')
        return 0;
    while (s[i])
    {
        if (s[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}


int builtin_pwd(char **args, t_minishell *sh)
{
    char buffer[4096];

    (void)args;
    if (getcwd(buffer, sizeof(buffer)))
        printf("%s\n", buffer);
    else
        perror("pwd");

    sh->exit_status = 0;
    return (0);
}
int builtin_env(char **args, t_minishell *sh)
{
    t_env *e = sh->env;

    (void)args;
    while (e)
    {
        if (e->value) // env n'affiche pas les variables sans valeur
            printf("%s=%s\n", e->key, e->value);
        e = e->next;
    }

    sh->exit_status = 0;
    return (0);
}
int builtin_cd(char **args, t_minishell *sh)
{
    char *path;

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
int builtin_exit(char **args, t_minishell *sh)
{
    long code;

    printf("exit\n");

    // Aucun argument → on quitte avec le dernier exit_status
    if (!args[1])
    {
        sh->should_exit = 1;
        sh->exit_status = sh->exit_status;
        return 0;
    }

    // Argument non numérique → erreur bash
    if (!ft_isnumber(args[1]))
    {
        printf("minishell: exit: %s: numeric argument required\n", args[1]);
        sh->should_exit = 1;
        sh->exit_status = 255;
        return 0;
    }

    // Argument valide → conversion
    code = ft_atol(args[1]);

    sh->should_exit = 1;
    sh->exit_status = (unsigned char)code;

    return 0;
}
