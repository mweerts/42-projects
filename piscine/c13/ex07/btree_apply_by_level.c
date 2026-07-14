/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_apply_by_level.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:12:05 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/23 19:56:04 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"
#include <stdlib.h>

t_list	*ft_create_elem(t_btree *root, int level)
{
	t_list	*elem;

	elem = (t_list *)malloc(sizeof(t_list));
	if (!elem)
		return (0);
	elem->root = root;
	elem->level = level;
	elem->next = 0;
	return (elem);
}

t_list	*ft_list_last(t_list *begin_list)
{
	while (begin_list->next)
		begin_list = begin_list->next;
	return (begin_list);
}

void	ft_list_push_back(t_list **begin_list, t_btree *root, int level)
{
	t_list	*elem;

	if (!(*begin_list))
	{
		*begin_list = ft_create_elem(root, level);
		return ;
	}
	elem = ft_create_elem(root, level);
	if (!elem)
		return ;
	ft_list_last(*begin_list)->next = elem;
}

void	btree_apply_by_level(t_btree *root,
	void (*applyf)(void *item, int current_level, int is_first))
{
	t_list	*queue;
	int		is_first;
	int		prev_level;

	if (!root)
		return ;
	queue = ft_create_elem(root, 0);
	prev_level = 0;
	is_first = 1;
	while (queue)
	{
		if (prev_level < queue->level)
		{
			is_first = 1;
			prev_level = queue->level;
		}
		applyf(queue->root->item, queue->level, is_first);
		if (queue->root->left)
			ft_list_push_back(&queue, queue->root->left, queue->level + 1);
		if (queue->root->right)
			ft_list_push_back(&queue, queue->root->right, queue->level + 1);
		is_first = 0;
		queue = queue->next;
	}
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

void func(void *item, int current_level, int is_first)
{
	printf("Value : %d || Level : %d || is_first : %d\n", 
				*(int*)item, current_level, is_first);
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

	btree_apply_by_level(root, &func);
}
*/
