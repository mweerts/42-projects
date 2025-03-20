/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:50:47 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/20 19:59:53 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	leave(t_data *data)
{
	// free_all
	(void)data;
	exit(0);
}

int	key_pressed(int keycode, t_data *data)
{
	if (keycode == K_ESC)
		leave(data);
	return (0);
}
