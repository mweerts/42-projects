/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:53:10 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 17:28:28 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

char	*ft_get_input(char *input)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (ft_char_isspace(input[i]))
		i++;
	if (input[i] == '-')
		return (0);
	if (input[i] == '+')
		i++;
	if (!(input[i] >= '0' && input[i] <= '9'))
		return (0);
	while (input[i] == '0' && input[i + 1])
		i++;
	while (input[i + j] >= '0' && input[i + j] <= '9')
		j++;
	if (input[i + j] != '\0')
		return (0);
	ret = ft_strndup(&input[i], j);
	if (!ret)
		return (0);
	return (ret);
}

int	ft_raise_error(char *str)
{
	ft_putstr(str);
	return (0);
}

int	main(int argc, char **argv)
{
	char	*input;
	char	*dct_path;
	t_list	*dictionnary;
	t_list	*multipliers;

	dictionnary = 0;
	multipliers = 0;
	dct_path = "numbers.dict";
	if (argc < 2 || argc > 3)
		return (ft_raise_error("Error\n"));
	if (argc == 3)
		dct_path = argv[1];
	input = ft_get_input(argv[argc - 1]);
	if (!input)
		return (ft_raise_error("Error\n"));
	if (!ft_parse_dict(dct_path, &dictionnary, &multipliers))
	{
		free(input);
		ft_free_dict_and_mult(dictionnary, multipliers);
		return (ft_raise_error("Dict Error\n"));
	}
	ft_print(input, dictionnary, multipliers);
	free(input);
	ft_free_dict_and_mult(dictionnary, multipliers);
	return (0);
}
