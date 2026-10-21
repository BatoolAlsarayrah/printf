/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: balsaray <balsaray@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 11:16:15 by balsaray          #+#    #+#             */
/*   Updated: 2026/10/20 15:58:33 by balsaray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft/libft.h"
# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	ft_putnbr_base(unsigned long n, char *base);
int	ft_putstr(char *str);
int	ft_putchar(char c);
int	ft_putnbr_dec(int n);
int	ft_putptr(void *ptr);
int	ft_putundef(char c);

#endif
