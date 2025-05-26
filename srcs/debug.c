/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:36:26 by lagea             #+#    #+#             */
/*   Updated: 2025/05/26 15:06:22 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void debug_print_argList(t_arg argList)
{
	ft_printf(2, "Flags: \n");
	ft_printf(2, "\tall: %b\n", argList.all);
	ft_printf(2, "\treverse: %b\n", argList.reverse);
	ft_printf(2, "\tsort_time: %b\n", argList.sort_time);
	ft_printf(2, "\trecursive: %b\n", argList.recurisve);
	ft_printf(2, "\tlong_format: %b\n", argList.long_format);
	ft_printf(2, "\tpath: %s\n", argList.path);
	ft_printf(2, "\textended_attributes: %b\n", argList.extended_attributes);
	ft_printf(2, "\tsort_size: %b\n", argList.sort_size);
	ft_printf(2, "\tblock_size: %b\n", argList.block_size);
	ft_printf(2, "\tnot_sort: %b\n", argList.not_sort);
	ft_printf(2, "\tno_name: %b\n", argList.no_name);
	ft_printf(2, "\tid: %b\n", argList.id);
	ft_printf(2, "\tslash: %b\n", argList.slash);
	ft_printf(2, "\toneline: %b\n", argList.oneline);
	ft_printf(2, "\tcomma: %b\n", argList.slash);
	ft_printf(2, "\thorizontal: %b\n", argList.horizontal);
	
	ft_printf(2, "\tall_path: \n");
	int i = 0;
	while (argList.all_path[i] != NULL) {
		ft_printf(2, "\t\t'%s'\n", argList.all_path[i]);
		i++;
	}
}

void debug_print_dirent(struct dirent *entry)
{
	ft_printf(1, "type: %d	name: %s", entry->d_type, entry->d_name);
	// ft_printf(1, "name len: \n", entry->d_namlen);
}

void debug_printFormatStruct(t_format *format)
{
	ft_printf(1, "max_user: %d\n", format->max_user);
	ft_printf(1, "max_group: %d\n", format->max_group);
	ft_printf(1, "max_link: %d\n", format->max_link);
	ft_printf(1, "max_size_bytes: %d\n", format->max_size_bytes);
	ft_printf(1, "max_name: %d\n", format->max_name);
	ft_printf(1, "max_block_size: %d\n", format->max_block_size);
	ft_printf(1, "max_uid: %d\n", format->max_uid);
	ft_printf(1, "max_gid: %d\n", format->max_gid);
}

void debug_printInfoStruct(t_info *info)
{
	ft_printf(1, "user_name: %s\n", info->user_name);
	ft_printf(1, "group_name: %s\n", info->group_name);
	ft_printf(1, "perm: %s\n", info->perm);
	ft_printf(1, "time: %s\n", info->time);
	ft_printf(1, "nlink: %d\n", info->nlink);
	ft_printf(1, "size_bytes: %d\n", info->size_bytes);
}

void debug_printNodeLs(t_ls *node)
{
	ft_printf(1, "relative_path: %s\n", node->relative_path);
	ft_printf(1, "name: %s\n", node->name);
	ft_printf(1, "type: %d\n", node->type);
	ft_printf(1, "symbolic: %b\n", node->is_symbolic);
	ft_printf(1, "directory: %b\n", node->is_dir);
	ft_printf(1, "format: %s\n", node->format);
	debug_printInfoStruct(node->info);
	// debug_printFormatStruct(node->format_info);
	if (node->subdir && node->subdir->head != NULL) {
		ft_printf(1, "subdir: \n");
		t_node *sub = node->subdir->head;
		while (sub != NULL) {
			// debug_printNodeLs(sub->content);
			sub = sub->next;
		}
	} else
		ft_printf(1, "no subdir\n");
}

void debug_print_subdir(void *content)
{
	t_subdir *subdir = content;
	ft_printf(1, "subdir name: %s\n", subdir->name);
}

void debug_print_colors_map(t_color_map *colors)
{
	ft_printf(1, "di: '%s'\n", colors->di);
	ft_printf(1, "ln: '%s'\n", colors->ln);
	ft_printf(1, "so: '%s'\n", colors->so);
	ft_printf(1, "pi: '%s'\n", colors->pi);
	ft_printf(1, "ex: '%s'\n", colors->ex);
	ft_printf(1, "bd: '%s'\n", colors->bd);
	ft_printf(1, "cd: '%s'\n", colors->cd);
	ft_printf(1, "fi: '%s'\n", colors->fi);
}

void debug_print_terminal_size(struct winsize *w)
{
	ft_printf(1, "ws_col: %d\n", w->ws_col);
	ft_printf(1, "ws_row: %d\n", w->ws_row);
	ft_printf(1, "ws_xpixel: %d\n", w->ws_xpixel);
	ft_printf(1, "ws_ypixel: %d\n", w->ws_ypixel);
}