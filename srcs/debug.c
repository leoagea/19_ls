/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:36:26 by lagea             #+#    #+#             */
/*   Updated: 2025/04/01 18:48:54 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void print_argList(t_arg argList)
{
	ft_printf(2, "Flags: \n");
	ft_printf(2, "\tall: %b\n", argList.all);
	ft_printf(2, "\treverse: %b\n", argList.reverse);
	ft_printf(2, "\tsort_time: %b\n", argList.sort_time);
	ft_printf(2, "\trecursive: %b\n", argList.recurisve);
	ft_printf(2, "\tlong_format: %b\n", argList.long_format);
	ft_printf(2, "\tpath: %s\n", argList.path);
	ft_printf(2, "\tall_path: \n");
	int i = 0;
	while (argList.all_path[i] != NULL){
		ft_printf(2, "\t\t%s\n", argList.all_path[i]);
		i++;
	}
}

void printAllElementsLsNode(t_ls_node *node)
{
	ft_printf(1, "perm: %s   ", node->perm);
	ft_printf(1, "date: %s	sym:%b   ", node->last_mod, node->symbolic);
	ft_printf(1, "size: %d   gid: %s   uid: %s   ", node->size_bytes, node->group_name, node->user_name);
	print_dirent(node->entry);
	if (node->symbolic)
		ft_printf(1, "     sym name: %s   ", node->sym_name);
	ft_printf(1, "\n");
}

void print_dirent(struct dirent *entry)
{
	ft_printf(1, "type: %d	name: %s", entry->d_type, entry->d_name);
	// ft_printf(1, "name len: \n", entry->d_namlen);
}

void printFormatStruct(t_format *format)
{
	ft_printf(1, "max_user: %d\n", format->max_user);
	ft_printf(1, "max_group: %d\n", format->max_group);
	ft_printf(1, "max_link: %d\n", format->max_link);
	ft_printf(1, "max_size_bytes: %d\n", format->max_size_bytes);
}