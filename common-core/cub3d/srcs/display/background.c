/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:03:56 by maxweert          #+#    #+#             */
/*   Updated: 2025/03/28 20:55:48 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	set_background(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			if (y > (HEIGHT / 2))
				draw_pixel(&data->s_img, x, y, data->map->floor_color);
			else
				draw_pixel(&data->s_img, x, y, data->map->ceiling_color);
			x++;
		}
		y++;
	}
}

void	set_textured_background(t_data *data)
{
	for(int y = HEIGHT / 2; y < HEIGHT; y++)
    {
		
      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = data->player.dir_x - data->player.plane_x;
      float rayDirY0 = data->player.dir_y - data->player.plane_y;
      float rayDirX1 = data->player.dir_x + data->player.plane_x;
      float rayDirY1 = data->player.dir_y + data->player.plane_y;
		int texWidth = 64;
		int texHeight = 64;
      // Current y position compared to the center of the screen (the horizon)
      int p = y - HEIGHT / 2;

      // Vertical position of the camera.
      float posZ = 0.5 * HEIGHT;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      float rowDistance = posZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WIDTH;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WIDTH;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = data->player.pos_x + rowDistance * rayDirX0;
      float floorY = data->player.pos_y + rowDistance * rayDirY0;

      for(int x = 0; x < WIDTH; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
        int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        int color;

        // floor
		unsigned char *src = (unsigned char *)&data->tex[TEX_FLOOR]->img.addr[ty * data->tex[TEX_FLOOR]->img.line_length + tx * (data->tex[TEX_FLOOR]->img.bits_per_pixel / 8)];
		color = *(unsigned int *)src;
        draw_pixel(&data->s_img, x, y, color);

        //ceiling (symmetrical, at screenHeight - y - 1 instead of y)
		src = (unsigned char *)&data->tex[TEX_CEILING]->img.addr[ty * data->tex[TEX_CEILING]->img.line_length + tx * (data->tex[TEX_CEILING]->img.bits_per_pixel / 8)];
		color = *(unsigned int *)src;
        draw_pixel(&data->s_img, x, HEIGHT - y - 1, color);	
      }
    }
}