/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:18:02 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/13 19:18:03 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int main(int ac, char **argv)
{
	t_data data;
	(void)ac;
	
	data_init(&data);
	if (parse_arguments(&data, ac, argv))
		exit_with_error(NULL, &data);
	print_data(&data);
	clean_up(&data);
	return (0);
}