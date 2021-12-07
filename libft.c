/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocmarout <ocmarout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 13:04:13 by ocmarout          #+#    #+#             */
/*   Updated: 2021/12/07 14:25:02 by ocmarout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(char const *str)
{
	int		i;
	int		sign;
	long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		sign = (0 * i++) - 1;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		nb = (nb * 10) + str[i] - '0';
		if (nb < 0 && sign == 1)
			return (-1);
		else if (nb < 0 && sign != 1)
			return (0);
		i++;
	}
	return ((int)nb * sign);
}

void	trim_spaces(char *s)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (s[j])
	{
		if (s[j] != ' ')
		{
			s[i++] = s[j++];
			k = 1;
		}
		else if (k && s[j] == ' ')
		{
			s[i++] = s[j++];
			k = 0;
		}
		else
			j++;
	}
	s[i] = 0;
	if (i > 0 && s[i - 1] == ' ')
		s[i - 1] = 0;
}

static char	*ft_write_str(int len, unsigned int mod, unsigned int nb)
{
	int		i;
	int		sign;
	char	*str;

	i = 0;
	if (len < 0)
	{
		sign = -1;
		len *= -1;
	}
	else
		sign = 1;
	str = malloc(sizeof(char) * len);
	if (!str)
		return (0);
	if (sign == -1)
		str[i++] = '-';
	while (mod > 0)
	{
		str[i++] = nb / mod + '0';
		nb %= mod;
		mod /= 10;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	int				len;
	long			mod;
	unsigned int	nb;

	mod = 1;
	len = 2;
	if (n < 0)
		len++;
	if (n < 0)
		nb = (unsigned)n * -1;
	else
		nb = (unsigned)n;
	while (mod * 10 <= nb)
	{
		len++;
		mod *= 10;
	}
	if (n < 0)
		len *= -1;
	return (ft_write_str(len, mod, nb));
}
