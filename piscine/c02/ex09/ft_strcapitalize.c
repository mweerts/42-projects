/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcapitalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:34:10 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/05 18:27:31 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_char_is_uppercase(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

int	ft_char_is_lowercase(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	return (0);
}

int	ft_char_is_alphanum(char c)
{
	if (ft_char_is_lowercase(c) || ft_char_is_uppercase(c)
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

char	*ft_strcapitalize(char *str)
{
	int	i;
	int	last_char;

	i = 0;
	last_char = ' ';
	while (str[i])
	{
		if (!ft_char_is_alphanum(last_char) && ft_char_is_lowercase(str[i]))
			str[i] -= 32;
		else if (ft_char_is_alphanum(last_char) && ft_char_is_uppercase(str[i]))
			str[i] += 32;
		last_char = str[i];
		i++;
	}
	return (str);
}
