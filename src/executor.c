#include "../include/minishell.h"

void executor(t_cmd *cmds, t_minishell *sh)
{
    if (!cmds)
        return;
    if (!cmds->next)
        exec_single(cmds, sh);
    else
        exec_pipeline(cmds, sh);
}
void exec_single(t_cmd *cmd, t_minishell *sh)
{
    pid_t pid;
    int   status;

    if (is_builtin(cmd->args[0]))
    {
        if (apply_redirs(cmd) == -1)
            return;
        exec_builtin(cmd, sh);   // dans le parent
        return;
    }
    pid = fork();
    if (pid == -1)
        return; // à améliorer plus tard

    if (pid == 0)
{
    // 🔥 ENFANT : signaux par défaut
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    if (apply_redirs(cmd) == -1)
        exit(1);

    exec_external(cmd, sh);
    exit(127);
}
else
{
    // 🔥 PARENT : ignorer les signaux pendant l’attente
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    waitpid(pid, &status, 0);

    // 🔥 restaurer les signaux du parent (readline)
    init_signals();

    if (WIFEXITED(status))
        sh->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        sh->exit_status = 128 + WTERMSIG(status);
}
}

void exec_pipeline(t_cmd *cmds, t_minishell *sh)
{
    int   prev_fd = -1;
    int   pipefd[2];
    pid_t last_pid = -1; 
    pid_t pid;
    int   status;

    while (cmds)
    {
        if (cmds->next && pipe(pipefd) == -1)
            return;

        pid = fork();
        if (pid == -1)
            return;

        if (pid == 0)
        {
            // 🔥 ENFANT : signaux par défaut
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            if (apply_redirs(cmds) == -1)
                exit(1);
            // entrée du pipe précédent
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // sortie vers le pipe suivant
            if (cmds->next)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            

            if (is_builtin(cmds->args[0]))
            {
                exec_builtin(cmds, sh);
                exit(sh->exit_status);
            }

            exec_external(cmds, sh);
            exit(127);
        }
        else
        {
            
            if (cmds->next == NULL)
                last_pid = pid;
            // 🔥 PARENT : ignorer les signaux pendant l’exécution
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);

            // fermer l’entrée précédente
            if (prev_fd != -1)
                close(prev_fd);

            // préparer la prochaine commande
            if (cmds->next)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }

            cmds = cmds->next;
        }
    }

    // attendre tous les enfants
    // attendre tous les enfants pour éviter les zombies
    while (wait(NULL) > 0)
        ;

    // récupérer l’état de la DERNIÈRE commande du pipeline
    if (last_pid != -1 && waitpid(last_pid, &status, 0) != -1)
    {
        if (WIFEXITED(status))
            sh->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            sh->exit_status = 128 + WTERMSIG(status);
    }
    init_signals();
}


int apply_redirs(t_cmd *cmd)
{
    t_redir *r = cmd->redirs;
    int      fd;

    while (r)
    {
        if (r->type == HEREDOC)
        {
            fd = open_heredoc(r->file);
            if (fd < 0)
                return (-1);
            dup2(fd, STDIN_FILENO);
            close(fd);
            r = r->next;
            continue;
        }

        if (r->type == REDIR_IN)
            fd = open(r->file, O_RDONLY);
        else if (r->type == REDIR_OUT)
            fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (r->type == APPEND)
            fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (fd < 0)
            return (-1);

        if (r->type == REDIR_IN)
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);

        close(fd);
        r = r->next;
    }
    return (0);
}

#include <sys/stat.h>   // pour stat() et S_ISDIR()

void exec_external(t_cmd *cmd, t_minishell *sh)
{
    char *cmdpath;

    if (!cmd->args || !cmd->args[0])
        exit(0);

    // Cas 1 : chemin explicite
    if (ft_strchr(cmd->args[0], '/'))
    {
        struct stat st;

        // Vérifier si c'est un dossier
        if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
        {
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": Is a directory\n", 17);
            exit(126);
        }

        // Vérifier si le fichier existe et est exécutable
        if (access(cmd->args[0], X_OK) != 0)
        {
            perror(cmd->args[0]);
            exit(126);
        }

        // Essayer d'exécuter
        execve(cmd->args[0], cmd->args, sh->envp);

        // Si execve échoue → permission denied ou autre
        perror(cmd->args[0]);
        exit(126);
    }

    // Cas 2 : recherche dans PATH
    cmdpath = path(cmd->args[0], sh->envp);
    if (!cmdpath)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": command not found\n", 21);
        exit(127);
    }

    if (access(cmdpath, X_OK) != 0)
    {
        perror(cmd->args[0]);
        free(cmdpath);
        exit(126);
    }

    execve(cmdpath, cmd->args, sh->envp);
    perror(cmd->args[0]);
    free(cmdpath);
    exit(127);
}




char **env_to_tab(t_env *env)
{
    int     size = 0;
    t_env   *tmp = env;
    char  **tab;
    int     i = 0;
    char    *key_value;

    // Compter les variables
    while (tmp)
    {
        size++;
        tmp = tmp->next;
    }

    tab = malloc(sizeof(char *) * (size + 1));
    if (!tab)
        return NULL;

    tmp = env;
    while (tmp)
    {
        // Construire "KEY=VALUE"
        if (tmp->value)
        {
            char *tmp2 = ft_strjoin(tmp->key, "=");
            key_value = ft_strjoin(tmp2, tmp->value);
            free(tmp2);
        }
        else
        {
            // Cas KEY sans valeur → "KEY"
            key_value = ft_strdup(tmp->key);
        }

        tab[i] = key_value;
        i++;
        tmp = tmp->next;
    }

    tab[i] = NULL;
    return tab;
}


int open_heredoc(char *limiter)
{
    int     pipefd[2];
    char    *line;

    if (pipe(pipefd) == -1)
        return (-1);

    // 🔥 HEREDOC : signaux spécifiques
    signal(SIGINT, SIG_DFL);   // Ctrl-C doit interrompre le heredoc
    signal(SIGQUIT, SIG_IGN);  // Ctrl-\ ignoré

    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            // Ctrl-C → readline renvoie NULL
            close(pipefd[1]);
            close(pipefd[0]);
            return (-1);       // indique interruption
        }

        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }

        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    close(pipefd[1]);

    // 🔥 restaurer les signaux du parent
    init_signals();

    return pipefd[0];
}
