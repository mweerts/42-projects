/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_apply_suffix.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:16:35 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/17 18:18:55 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"

void	btree_apply_suffix(t_btree *root, void (*applyf)(void *))
{
	if (!root)
		return ;
	btree_apply_suffix(root->left, applyf);
	btree_apply_suffix(root->right, applyf);
	applyf(root->item);
}

/*
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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

void	ft_putstr(void *str)
{
	int i;
	char *string;

	i = 0;
	string = (char*)str;
	while (string[i])
		i++;
	write(1, string, i);
	write(1, "\n", 1);
}

int main()
{
	t_btree *root;

	root = btree_create_node("root");
	root->left = btree_create_node("root.left");
	root->right = btree_create_node("root.right");
	root->left->left = btree_create_node("root.left.left");
	root->left->right = btree_create_node("root.left.right");
	root->right->right = btree_create_node("root.right.right");
	root->right->left = btree_create_node("root.right.left");

	btree_apply_suffix(root, &ft_putstr);
}
*/