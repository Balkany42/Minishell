#include "../include/minishell.h"

char *ft_strdup(char *src)
{
    int i = 0;
    char *dest;

    while(src[i])
        i++;
    dest = malloc(i + 1);
    if(!dest)
        return(NULL);
    i = 0;
    while(src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return(dest);
}

int ft_isalpha(int c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return (1);
    return (0);
}
int ft_isalnum(int c)
{
    if ((c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z'))
        return (1);
    return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *ft_strchr(const char *s, int c)
{
    int i = 0;

    while (s[i])
    {
        if (s[i] == (char)c)
            return ((char *)&s[i]);
        i++;
    }
    if (c == '\0')
        return ((char *)&s[i]);
    return (NULL);
}

char *ft_substr(const char *s, unsigned int start, size_t len)
{
    char    *sub;
    size_t  i;

    if (!s)
        return (NULL);

    // Si start dépasse la longueur → renvoie une chaîne vide
    size_t slen = 0;
    while (s[slen])
        slen++;

    if (start >= slen)
    {
        sub = malloc(1);
        if (!sub)
            return (NULL);
        sub[0] = '\0';
        return (sub);
    }

    // Ajuster len si nécessaire
    if (start + len > slen)
        len = slen - start;

    sub = malloc(len + 1);
    if (!sub)
        return (NULL);

    i = 0;
    while (i < len)
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';

    return (sub);
}
size_t ft_strlen(const char *s)
{
    size_t i = 0;

    if (!s)
        return (0);

    while (s[i])
        i++;
    return (i);
}

int  count_digits(int n)
{
    int count = 1;

    while (n / 10 != 0)
    {
        n /= 10;
        count++;
    }
    return count;
}

char *ft_itoa(int n)
{
    long    nb = n;
    int     len = (nb <= 0) ? 1 : 0;
    long    tmp = nb;

    while (tmp)
    {
        tmp /= 10;
        len++;
    }

    char *str = malloc(len + 1);
    if (!str)
        return NULL;

    str[len] = '\0';

    if (nb == 0)
    {
        str[0] = '0';
        return str;
    }

    if (nb < 0)
    {
        str[0] = '-';
        nb = -nb;
    }

    while (nb)
    {
        str[--len] = (nb % 10) + '0';
        nb /= 10;
    }

    return str;
}

char *ft_strjoin(const char *s1, const char *s2)
{
    size_t  len1 = ft_strlen(s1);
    size_t  len2 = ft_strlen(s2);
    char    *res;
    size_t  i = 0;
    size_t  j = 0;

    res = malloc(len1 + len2 + 1);
    if (!res)
        return NULL;

    while (i < len1)
    {
        res[i] = s1[i];
        i++;
    }
    while (j < len2)
    {
        res[i + j] = s2[j];
        j++;
    }
    res[i + j] = '\0';

    return res;
}
int ft_isnumber(const char *s)
{
    int i = 0;

    if (!s || !s[0])
        return 0;

    // Signe optionnel
    if (s[i] == '+' || s[i] == '-')
        i++;

    // Doit avoir au moins un chiffre
    if (!s[i])
        return 0;

    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return 0;
        i++;
    }

    return 1;
}
long ft_atol(const char *s)
{
    long result = 0;
    int  sign = 1;
    int  i = 0;

    if (!s)
        return 0;

    // Signe
    if (s[i] == '-' || s[i] == '+')
    {
        if (s[i] == '-')
            sign = -1;
        i++;
    }

    while (s[i] >= '0' && s[i] <= '9')
    {
        result = result * 10 + (s[i] - '0');
        i++;
    }

    return result * sign;
}
int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    if (n == 0)
        return 0;

    i = 0;
    while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
        i++;

    if (i == n)
        return 0;

    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void    free_env_list(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}
