#include "pipex.h"

// Y'a encore des for que j'ai copié collé là dedans

int builtin_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    if (argv[1] && argv[1][0] == '-' && argv[1][1] == 'n')
    {
        int j = 1;
        while (argv[1][j] == 'n')
            j++;
        if (argv[1][j] == '\0')
        {
            newline = 0;
            i = 2;
        }
    }

    while (argv[i])
    {
        ft_printf("%s", argv[i]);
        if (argv[i + 1])
            ft_printf(" ");
        i++;
    }
    if (newline)
        ft_printf("\n");
    return (0);
}
int builtin_pwd(void)
{
    char buffer[4096];

    if (getcwd(buffer, sizeof(buffer)))
        ft_printf("%s\n", buffer);
    return (0);
}
int builtin_env(char **envp)
{
    for (int i = 0; envp[i]; i++)
        ft_printf("%s\n", envp[i]);
    return (0);
}
int builtin_cd(char **argv)
{
    if (!argv[1])
        return chdir(getenv("HOME"));

    if (chdir(argv[1]) != 0)
    {
        perror("cd");
        return 1;
    }
    return 0;
}
int builtin_exit(char **argv)
{
    int code = 0;

    if (argv[1])
        code = atoi(argv[1]);

    exit(code);
}
int builtin_export(char **argv, t_env **env)
{
    if (!argv[1])
    {
        // export sans arguments → afficher trié
        print_export_sorted(*env);
        return 0;
    }

    if (!is_valid_key(argv[1]))
    {
        printf("minishell: export: `%s`: not a valid identifier\n", argv[1]);
        return 1;
    }

    char *eq = strchr(argv[1], '=');

    if (!eq)
    {
        // export KEY (sans valeur)
        env_set(env, argv[1], NULL);
        return 0;
    }

    char *key = ft_substr(argv[1], 0, eq - argv[1]);
    char *value = ft_strdup(eq + 1);

    env_set(env, key, value);

    free(key);
    free(value);
    return 0;
}


int builtin_unset(char **argv, t_env **env)
{
    if (!argv[1])
        return 0;

    t_env *cur = *env;
    t_env *prev = NULL;

    while (cur)
    {
        if (!strcmp(cur->key, argv[1]))
        {
            if (prev)
                prev->next = cur->next;
            else
                *env = cur->next;

            free(cur->key);
            free(cur->value);
            free(cur);
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}
int is_valid_key(char *s)
{
    if (!ft_isalpha(s[0]) && s[0] != '_')
        return 0;
    for (int i = 1; s[i] && s[i] != '='; i++)
        if (!ft_isalnum(s[i]) && s[i] != '_')
            return 0;
    return 1;
}
void env_set(t_env **env, char *key, char *value)
{
    t_env *e = env_find(*env, key);

    if (e)
    {
        free(e->value);
        e->value = value ? ft_strdup(value) : NULL;
        return;
    }

    t_env *new = env_new(key, value);
    new->next = *env;
    *env = new;
}


int is_builtin(char *cmd)
{
    return (!strcmp(cmd, "echo")
        || !strcmp(cmd, "cd")
        || !strcmp(cmd, "pwd")
        || !strcmp(cmd, "export")
        || !strcmp(cmd, "unset")
        || !strcmp(cmd, "env")
        || !strcmp(cmd, "exit"));
}
int exec_builtin(char **argv, char ***envp)
{
    if (!strcmp(argv[0], "echo"))
        return builtin_echo(argv);
    if (!strcmp(argv[0], "cd"))
        return builtin_cd(argv);
    if (!strcmp(argv[0], "pwd"))
        return builtin_pwd();
    if (!strcmp(argv[0], "env"))
        return builtin_env(*envp);
    if (!strcmp(argv[0], "exit"))
        return builtin_exit(argv);
    if (!strcmp(argv[0], "export"))
        return builtin_export(argv, envp);
    if (!strcmp(argv[0], "unset"))
        return builtin_unset(argv, envp);

    return 0;
}

// Les deux fonctions suivantes transforment l'env en liste chainée
t_env *env_new(char *key, char *value)
{
    t_env *e = malloc(sizeof(t_env));
    e->key = ft_strdup(key);
    e->value = value ? ft_strdup(value) : NULL;
    e->next = NULL;
    return e;
}

t_env *env_from_envp(char **envp)
{
    t_env *list = NULL;
    t_env *last = NULL;

    for (int i = 0; envp[i]; i++)
    {
        char *eq = strchr(envp[i], '=');
        char *key = ft_substr(envp[i], 0, eq - envp[i]);
        char *value = ft_strdup(eq + 1);

        t_env *new = env_new(key, value);
        free(key);
        free(value);

        if (!list)
            list = new;
        else
            last->next = new;
        last = new;
    }
    return list;
}
// La fonction suivante sert à trovuer une variable

t_env *env_find(t_env *env, char *key)
{
    while (env)
    {
        if (!strcmp(env->key, key))
            return env;
        env = env->next;
    }
    return NULL;
}
void print_export_sorted(t_env *env)
{
    // 1) compter
    int count = 0;
    for (t_env *e = env; e; e = e->next)
        count++;

    // 2) tableau
    t_env *arr[count];
    int i = 0;
    for (t_env *e = env; e; e = e->next)
        arr[i++] = e;

    // 3) tri
    for (int a = 0; a < count - 1; a++)
        for (int b = a + 1; b < count; b++)
            if (strcmp(arr[a]->key, arr[b]->key) > 0)
            {
                t_env *tmp = arr[a];
                arr[a] = arr[b];
                arr[b] = tmp;
            }

    // 4) affichage
    for (int j = 0; j < count; j++)
    {
        printf("declare -x %s", arr[j]->key);
        if (arr[j]->value)
            printf("=\"%s\"", arr[j]->value);
        printf("\n");
    }
}
