/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:36:26 by lagea             #+#    #+#             */
/*   Updated: 2025/04/22 18:09:24 by lagea            ###   ########.fr       */
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

void printInfoStruct(t_info *info)
{
	ft_printf(1, "user_name: %s\n", info->user_name);
	ft_printf(1, "group_name: %s\n", info->group_name);
	ft_printf(1, "perm: %s\n", info->perm);
	ft_printf(1, "last_mod: %s\n", info->last_mod);
	ft_printf(1, "nlink: %d\n", info->nlink);
	ft_printf(1, "size_bytes: %d\n", info->size_bytes);
}

void printNodeLs(t_ls *node)
{
	ft_printf(1, "relative_path: %s\n", node->relative_path);
	ft_printf(1, "name: %s\n", node->name);
	ft_printf(1, "type: %d\n", node->type);
	ft_printf(1, "symbolic: %b\n", node->is_symbolic);
	ft_printf(1, "directory: %b\n", node->is_dir);
	ft_printf(1, "format: %s\n", node->format);
	printInfoStruct(node->info);
	printFormatStruct(node->format_info);
	if (node->subdir->head != NULL){
		ft_printf(1, "subdir: \n");
		t_node *sub = node->subdir->head;
		while (sub != NULL){
			// printNodeLs(sub->content);
			sub = sub->next;
		}
	}
	else
		ft_printf(1, "no subdir\n");
}

void print_subdir(void *content)
{
	t_subdir *subdir = content;
	ft_printf(1, "subdir name: %s\n", subdir->name);
}
