/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 22:43:10 by megrelli          #+#    #+#             */
/*   Updated: 2025/12/30 18:12:18 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_print_format(char format, va_list ap)
{
	int	len;

	len = 0;
	if (format == '%')
		len += write(1, "%", 1);
	else if (format == 'c')
		len += ft_printchar(va_arg(ap, int));
	else if (format == 's')
		len += ft_printstr(va_arg(ap, char *));
	else if (format == 'd' || format == 'i')
		len += ft_printnbr(va_arg(ap, int));
	else if (format == 'u')
		len += ft_printnbr_uns(va_arg(ap, unsigned int));
	else if (format == 'x' || format == 'X')
		len += ft_printhexa(va_arg(ap, unsigned int), format);
	else if (format == 'p')
		len += ft_printpointer(va_arg(ap, void *));
	else
		return (-1);
	return (len);
}

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		i;
	int		len;
	int		count;

	va_start(ap, str);
	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			count = ft_print_format(str[i + 1], ap);
			if (count == -1)
				return (-1);
			len += count;
			i += 2;
		}
		else
			len += write(1, &str[i++], 1);
	}
	va_end (ap);
	return (len);
}

/*int main()
{
	 void	*ptr = NULL;
	 char *s = NULL;
	 long	ld = 42;
	printf("%r %ld %c %p %d %u %x\n", s, ld, '?', ptr, -42, 420, 42);
	ft_printf("%s %ld %c %p %d %u %x\n", s, ld, '?', ptr, -42, 420, 42);
}*/
