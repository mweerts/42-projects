/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_search_item.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxweert <maxweert@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:36:51 by maxweert          #+#    #+#             */
/*   Updated: 2024/09/17 20:57:13 by maxweert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"

void	*btree_search_item(t_btree *root, void *data_ref,
			int (*cmpf)(void *, void *))
{
	if (!root)
		return (0);
	if (cmpf(root->item, data_ref) == 0)
		return (root->item);
	if (cmpf(root->item, data_ref) > 0)
		return (btree_search_item(root->left, data_ref, cmpf));
	return (btree_search_item(root->right, data_ref, cmpf));
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

int	ft_strcmp_len(void *str1, void *str2)
{
	int i = 0;
	int j = 0;

	char *s1 = (char*)str1;
	char *s2 = (char*)str2;
	while (s1[i])
		i++;
	while (s2[j])
		j++;
	return (i - j);
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
	btree_insert_data(&root, "abcd", &ft_strcmp_len);
	btree_insert_data(&root, "wqdqd", &ft_strcmp_len);
	btree_insert_data(&root, "abcasddqwd", &ft_strcmp_len);
	btree_insert_data(&root, "wds", &ft_strcmp_len);
	btree_insert_data(&root, "qdwqwdqwd", &ft_strcmp_len);
	btree_insert_data(&root, "dwdws", &ft_strcmp_len);

	printf("%s\n", (char*)btree_search_item(root, "abcsd", &ft_strcmp_len));
	//btree_apply_infix(root, &ft_putstr);
}
*/