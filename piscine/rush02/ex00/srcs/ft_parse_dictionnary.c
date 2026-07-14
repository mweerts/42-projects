/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_dictionnary.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 20:48:58 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/22 16:35:37 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush02.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	ft_add_dicts(char **tab, t_list **dictionnary, t_list **multipliers)
{
	char	*str;

	str = ft_parse_string(tab[1]);
	if (!str)
		return (0);
	if (str[0] == '\0' || !(ft_check_str_number(tab[0])))
	{
		free(str);
		return (0);
	}
	if (ft_check_str_number(tab[0]) == 2)
	{
		free(str);
		return (1);
	}
	tab[0] = ft_trim_number(tab[0]);
	if (ft_str_number_is_multiplier(tab[0]))
		ft_list_push_back(multipliers, ft_get_multiplier(tab[0]), str);
	else
		ft_list_push_back(dictionnary, ft_atoi(tab[0]), str);
	return (1);
}

int	ft_parse_line(char *line, t_list **dictionnary, t_list **multipliers)
{
	char	**tab;
	int		i;

	i = 0;
	tab = ft_split(line, ":");
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	if (i != 2)
	{
		free_tab(tab);
		return (0);
	}
	if (ft_add_dicts(tab, dictionnary, multipliers) == 0)
	{
		free_tab(tab);
		return (0);
	}
	free_tab(tab);
	return (1);
}

int	ft_parse_lines(char *file_content,
	t_list **dictionnary, t_list **multipliers)
{
	char	**tab;
	int		i;

	i = 0;
	tab = ft_split(file_content, "\n");
	if (!tab)
		return (0);
	while (tab[i])
	{
		if (!(ft_parse_line(tab[i], dictionnary, multipliers)))
		{
			free_tab(tab);
			free(file_content);
			return (0);
		}
		i++;
	}
	free(file_content);
	free_tab(tab);
	return (1);
}

int	ft_parse_dict(char *dict_path, t_list **dictionnary, t_list **multipliers)
{
	int		fd;
	int		ret;
	char	buffer[256];
	char	*file_content;

	fd = open(dict_path, O_RDONLY);
	if (fd == -1)
		return (0);
	ret = read(fd, buffer, 256);
	buffer[ret] = '\0';
	file_content = malloc(sizeof(char));
	file_content[0] = '\0';
	while (ret > 0)
	{
		file_content = ft_strjoin(file_content, buffer);
		ret = read(fd, buffer, 256);
		buffer[ret] = '\0';
	}
	if (close(fd) == -1)
		return (ft_free_file_content(file_content));
	if (!ft_parse_lines(file_content, dictionnary, multipliers))
		return (0);
	if (!ft_check_dict(dictionnary) || !ft_check_multipliers(multipliers))
		return (0);
	return (1);
}
