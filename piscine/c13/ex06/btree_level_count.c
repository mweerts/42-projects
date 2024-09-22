/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_level_count.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:32:19 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/18 16:07:55 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"

void	btree_level_count_recursive(t_btree *root, int *maxlevel, int currlevel)
{
	if (!root)
		return ;
	currlevel += 1;
	btree_level_count_recursive(root->left, maxlevel, currlevel);
	if (currlevel > *maxlevel)
		*maxlevel = currlevel;
	btree_level_count_recursive(root->right, maxlevel, currlevel);
}

int	btree_level_count(t_btree *root)
{
	int	max;

	max = 0;
	btree_level_count_recursive(root, &max, 0);
	return (max);
}

/*
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

t_btree	*btree_create_node(void *item)
{
	t_btree	*node;

	node = malloc(sizeof(t_btree));
	if (node == 0)
		return (0);
	node->item = item;
	node->left = 0;
	node->right = 0;
	return (node);
}

void	ft_putnb(void *nb)
{
	printf("%d\n", *(int*)nb);
}

int	ft_intcmp(void *a, void *b)
{
	return (*(int*)a - *(int*)b);
}

void	btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	btree_apply_infix(root->left, applyf);
	applyf(root->item);
	btree_apply_infix(root->right, applyf);
}


t_btree	*btree_insert_data_recursive(t_btree *root,
		void *item, int (*cmpf)(void *, void*))
{
	if (!root)
		return (btree_create_node(item));
	if (cmpf(root->item, item) > 0)
		root->left = btree_insert_data_recursive(root->left, item, cmpf);
	else
		root->right = btree_insert_data_recursive(root->right, item, cmpf);
	return (root);
}

void	btree_insert_data(t_btree **root, void *item,
		int (*cmpf)(void *, void *))
{
	t_btree	*head;

	head = *root;
	*root = btree_insert_data_recursive(*root, item, cmpf);
}

int main()
{
	t_btree *root = 0;
	int b = 2;
	int a = 3;
	int c = 1;
	int d = 7;
	int y = 8;
	int z = 9;

	btree_insert_data(&root, &a, &ft_intcmp);
	btree_insert_data(&root, &b, &ft_intcmp);
	btree_insert_data(&root, &c, &ft_intcmp);
	btree_insert_data(&root, &d, &ft_intcmp);
	btree_insert_data(&root, &y, &ft_intcmp);
	btree_insert_data(&root, &z, &ft_intcmp);

	btree_apply_infix(root, &ft_putnb);
	printf("%d\n", btree_level_count(root));
}
*/