/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_max_min.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 20:42:01 by llebugle          #+#    #+#             */
/*   Updated: 2025/04/01 20:42:02 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_max(double x, double y)
{
	if (x > y)
		return (x);
	return (y);
}

double	ft_min(double x, double y)
{
	if (x < y)
		return (x);
	return (y);
}
