/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:10:28 by llebugle          #+#    #+#             */
/*   Updated: 2025/03/27 18:10:29 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void set_minimap(t_data *data)
{
    int i, j;
    int radius;
    int center_x, center_y;
    int distance;
    
    // Set minimap size and position
    radius = WIDTH / 16;  // Half the width of the previous square
    center_x = WIDTH - radius - 20;
    center_y = HEIGHT - radius - 20;
    
    // Draw minimap background and border in one pass
    for (i = -radius; i <= radius; i++)
    {
        for (j = -radius; j <= radius; j++)
        {
            // Calculate distance from center
            distance = i*i + j*j;
            
            // Check if the pixel is within the circle
            if (distance <= radius*radius)
            {
                // Draw background for the circle
                draw_pixel(&data->s_img, center_x + j, center_y + i, 0x44000000);
                
                // Draw border for the circle if near the edge
                if (distance >= (radius-1)*(radius-1))
                {
                    draw_pixel(&data->s_img, center_x + j, center_y + i, CROSS_COLOR);
                }
            }
        }
    }
    
    // Draw player position at center
    for (i = -2; i <= 2; i++)
    {
        for (j = -2; j <= 2; j++)
        {
            if (i*i + j*j <= 4) // Circle with radius 2
            {
                draw_pixel(&data->s_img, center_x + j, center_y + i, 0xFFFF0000); // Red player dot
            }
        }
    }
}

int	render_minimap(t_data *data)
{
	set_minimap(data);
	return (0);
}

