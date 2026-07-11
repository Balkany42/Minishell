#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
    char        *line;
    t_token     *tokens;
    t_cmd       *cmds;
    t_minishell sh;

    (void)argc;
    (void)argv;

    // 🔥 INITIALISATION MINISHELL
    sh.env = init_env(envp);
    sh.envp = env_to_tab(sh.env);
    sh.exit_status = 0;
    sh.should_exit = 0;

    init_signals();

    while (1)
    {
        line = readprompt();
        if (!line)
        {
            write(1, "exit\n", 5);
            break;
        }

        if (!is_blank_line(line))
            add_history(line);

        /* ---------------- LEXER ---------------- */
        tokens = lexer(line);
        if (!tokens)
        {
            if (line[0] != '\0' && line[0] != ' ' && line[0] != '\t')
                printf("Syntax error: unclosed quote\n");
            free(line);
            continue;
        }

        /* ---------------- SYNTAX CHECK ---------------- */
        if (check_syntax_errors(tokens))
        {
            printf("Syntax error near token\n");
            sh.exit_status = 2;
            free_tokens(tokens);
            free(line);
            continue;
        }

        /* ---------------- PARSER (+ EXPANSION) ---------------- */
        cmds = parser(tokens, &sh);   // 🔥 parser fait l’expansion maintenant
        free_tokens(tokens);

        if (!cmds)
        {
            // parser a déjà mis exit_status si besoin
            free(line);
            continue;
        }

        /* ---------------- EXECUTOR ---------------- */
        executor(cmds, &sh);

        /* ---------------- FREE ---------------- */
        free_cmds(cmds);
        free(line);

        /* ---------------- REGENERER ENV TAB ---------------- */
        free_tab(sh.envp);
        sh.envp = env_to_tab(sh.env);

        if (sh.should_exit)
            break;
    }

    /* ---------------- FREE FINAL ---------------- */
    free_env_list(sh.env);
    free_tab(sh.envp);

    return 0;
}
