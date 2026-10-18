/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balsaray <balsaray@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 11:39:02 by balsaray          #+#    #+#             */
/*   Updated: 2026/10/18 11:40:56 by balsaray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	check_type(char c, va_list *list)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar(va_arg(*list, int));
	else if (c == 's')
		count += ft_putstr(va_arg(*list, char *));
	else if (c == 'p')
		count += ft_putptr(va_arg(*list, void *));
	else if (c == 'd')
		count += ft_putnbr_dec(va_arg(*list, int));
	else if (c == 'i')
		count += ft_putnbr_dec(va_arg(*list, int));
	else if (c == 'u')
		count += ft_putnbr_base(va_arg(*list, unsigned int), "0123456789");
	else if (c == 'x')
		count += ft_putnbr_base(va_arg(*list, unsigned int),
				"0123456789abcdef");
	else if (c == 'X')
		count += ft_putnbr_base(va_arg(*list, unsigned int),
				"0123456789ABCDEF");
	else if (c == '%')
		count += ft_putchar('%');
	return (count);
}
static int	formatcheck(const char *format)
{
	int	i;

	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] != 'c' && format[i] != 's' && format[i] != 'd'
				&& format[i] != 'i' && format[i] != 'x' && format[i] != 'X'
				&& format[i] != 'u' && format[i] != '%' && format[i] != 'p')
				return (0);
			i++;
		}
		else
			i++;
	}
	return (1);
}

int	ft_printf(const char *format, ...)
{
	va_list	list;
	int		count;
	int		i;

	if (!format)
		return (-1);
	if (!formatcheck(format))
		return (-1);
	va_start(list, format);
	count = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			count += check_type(format[i++], &list);
		}
		else
		{
			count += ft_putchar(format[i++]);
		}
	}
	va_end(list);
	return (count);
}
