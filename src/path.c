#include "../include/minishell.h"

char *join_path(const char *dir, const char *cmd)
{
    char *tmp;
    char *full;

    tmp = ft_strjoin(dir, "/");
    if (!tmp)
        return NULL;

    full = ft_strjoin(tmp, cmd);
    free(tmp);

    return full;
}

char *path(char *cmd, char **envp)
{
    char    *path_env;
    char    **paths;
    char    *full;
    int     i;

    if (!cmd || !cmd[0])
        return NULL;

    // 1) Si cmd contient un '/', c'est un chemin direct
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }

    // 2) Récupérer PATH dans envp
    path_env = NULL;
    for (i = 0; envp[i]; i++)
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    }

    // Si pas de PATH → impossible de trouver la commande
    if (!path_env)
        return NULL;

    // 3) Split PATH
    paths = ft_split(path_env, ':');
    if (!paths)
        return NULL;

    // 4) Tester chaque dossier
    i = 0;
    while (paths[i])
    {
        full = join_path(paths[i], cmd);
        if (!full)
        {
            i++;
            continue;
        }

        if (access(full, X_OK) == 0)
        {
            // Libérer le split
            int j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);

            return full; // trouvé
        }

        free(full);
        i++;
    }

    // 5) Rien trouvé → libérer et renvoyer NULL
    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);

    return NULL;
}
