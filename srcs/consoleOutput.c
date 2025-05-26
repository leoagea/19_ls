/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consoleOutput.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:44:36 by lagea             #+#    #+#             */
/*   Updated: 2025/05/26 16:56:39 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void print_xattr(t_dll *xattr_list)
{
	t_node	*current;
	t_xattr *attr;

	if (!xattr_list || !xattr_list->head)
		return;

	current = xattr_list->head;
	while (current) {
		attr = current->content;
		int tmp = (int)attr->value_size;
		if (attr)
			ft_printf(1, "\t%s\t%d\n", attr->name, tmp);
		current = current->next;
	}
	dll_free(xattr_list, freeXattr);
	xattr_list->head = NULL;
	xattr_list->tail = NULL;
	free(xattr_list);
	xattr_list = NULL;
}

void print_format(t_data *data, t_ls *ls)
{
	char *color = data->is_tty ? get_color_from_env(ls, data) : "";
	char *reset = data->is_tty ? COLOR_RESET : "";

	if (data->arg.long_format) {
		ft_printf(1, "%s", ls->format);
		ft_printf(1, "%s%s%s", color, ls->name, reset);

		if (ls->is_symbolic) {
			ft_printf(1, " -> %s", ls->info->sym_name);
		}

		if (ls->xattr_list && ls->xattr_list->head) {
			ft_printf(1, "\n");
			print_xattr(ls->xattr_list);
		}

	} else {
		ft_printf(1, "%s%s%s", color, ls->format, reset);
	}
}

static void print_horizontal(t_data *data, t_dll *list)
{
	t_node *node = list->head;
	if (!node || !node->content)
		return;

	int		col = data->w.ws_col;
	int		current_width = 0;
	bool	first_in_line = true;
	t_node *next = NULL;

	while (node) {
		t_ls *ls = node->content;
		if (!ls) {
			node = node->next;
			continue;
		}

		next = node->next;
		int name_width = ft_strlen(ls->name);
		int separator_width = 2;
		int total_width = name_width + (first_in_line ? 0 : separator_width);

		if (!first_in_line && (current_width + total_width >= col)) {
			ft_printf(1, ", ");
			ft_printf(1, "\n");
			current_width = name_width;
			first_in_line = true;
		} else {
			if (!first_in_line)
				ft_printf(1, ", ");
			current_width += total_width;
		}

		ft_printf(1, "%s%s%s", get_color_from_env(ls, data), ls->name,
				  COLOR_RESET);

		first_in_line = false;
		node = next;
	}

	ft_printf(1, "\n");
}

static void print_column(t_data *data, t_dll *list)
{
	if (data->arg.horizontal) {
		print_horizontal(data, list);
		return;
	}

	t_node *node = list->head;
	if (!node || !node->content)
		return;

	t_ls *ls = node->content;
	if (!ls) {
		printf("Warning: Uninitialized format_info\n");
		return;
	}

	int max_name = (int)get_max_len(list);
	if (max_name == 0)
		max_name = 1;
	
	// printf("ws_col: %d\n", data->w.ws_col);
	int nb_col = data->w.ws_col / max_name;
	if (nb_col == 0)
		nb_col = 1;

	if (list->size == 0) {
		printf("Warning: Empty list\n");
		return;
	}
	size_t rows = (list->size + nb_col - 1) / nb_col; // Round up division

	for (size_t i = 0; i < rows; i++) {
		t_node *current = node;
		for (int j = 0; j < nb_col && current; j++) {
			ls = current->content;
			if (ls) {
				if (data->arg.block_size){
					ft_printf(1, "%s", ls->format_block);
				}
				print_format(data, ls);
			}

			// Skip nb_col nodes ahead, but check for NULL
			for (size_t k = 0; k < rows && current; k++)
				current = current->next;
		}
		node = node->next;
		printf("\n");
	}

	// freeFormatStruct(&ls->format_info);
}

static void handle_subdirs(t_data *data, t_dll *printsubdir)
{
	if (!printsubdir->head)
		return;

	dll_bubble_sort(printsubdir->head, printsubdir->tail, compareSubdirName);
	if (data->arg.reverse)
		dll_revert(printsubdir);

	t_node *current = printsubdir->head;
	while (current) {
		t_subdir *tmp = current->content;
		if (tmp && tmp->subdir_list) {
			ft_printf(1, "\n%s:\n", tmp->path);
			print_recursive(data, tmp->subdir_list);
		}
		current = current->next;
	}
}

static void print_oneline(t_data *data, t_dll *list)
{
	if (!list || !list->head) {
		printf("Warning: Empty list\n");
		return;
	}

	t_node *node = list->head;
	while (node) {
		t_ls *ls = node->content;
		if (ls) {
			print_format(data, ls);
			ft_printf(1, "\n");
		}
		node = node->next;
	}
}

void print_recursive(t_data *data, t_dll *list)
{
	if (!list) {
		printf("Error: NULL list passed to print_recursive\n");
		return;
	}

	t_dll *printsubdir = malloc(sizeof(t_dll));
	if (!printsubdir) {
		printf("Error: Failed to allocate printsubdir\n");
		return;
	}
	dll_init(printsubdir);

	// Print current directory contents
	if (data->arg.long_format) {
		ft_printf(1, "%s %d\n", TOTAL_BLOCKS, calculateTotalBlocks(list));
		// Print in long format
		t_node *node = list->head;
		while (node) {
			if (node->content) {
				t_ls *ls = node->content;
				print_format(data, ls);
				printf("\n");
			}
			node = node->next;
		}
	} else if (!data->is_tty || data->arg.oneline) {
		print_oneline(data, list);
	} else {
		if (data->arg.horizontal)
			print_horizontal(data, list);
		else
			print_column(data, list);
	}

	// Collect subdirectories
	t_node *node = list->head;
	while (node) {
		if (node->content) {
			t_ls *ls = node->content;
			if (ls->is_dir && ls->subdir) {
				t_subdir *subdir = mallocSubdir();
				if (subdir) {
					subdir->name = ft_strdup(ls->name);
					subdir->lower_name = string_to_lower(subdir->name);
					subdir->path = ft_strdup(ls->relative_path);
					subdir->subdir_list = ls->subdir;
					dll_insert_tail(subdir, printsubdir);
				}
			}
		}
		node = node->next;
	}

	// Handle subdirectories recursively
	handle_subdirs(data, printsubdir);

	dll_free(printsubdir, freeSubdir);
	free(printsubdir);
}

static void print_direct(t_data *data, t_dll *list)
{
	if (!list) {
		printf("Error: NULL list passed to print_direct\n");
		return;
	}

	if (data->arg.long_format && !data->arg.directory)
		ft_printf(1, "%s %d\n", TOTAL_BLOCKS, calculateTotalBlocks(list));

	t_node *node = list->head;
	// t_node *head = list->head;
	while (node != NULL) {
		if (!node->content) {
			printf("Warning: Skipping NULL content node\n");
			node = node->next;
			continue;
		}

		t_ls *ls = node->content;
		print_format(data, ls);
		ft_printf(1, "\n");
		node = node->next;
	}
	// t_ls *head_ls = head->content;
	// freeFormatStruct(&head_ls->format_info);
}

void output(t_data *data, t_dll *list)
{
	if (list->head == NULL) {
		printf("null output\n");
		return;
	}
	
	if (!data->arg.not_sort) {		
		if (data->arg.sort_time || (data->arg.access_time && !data->arg.long_format))
			dll_bubble_sort(list->head, list->tail, compareTime);
		else if (data->arg.access_time && data->arg.sort_time && data->arg.long_format)
			dll_bubble_sort(list->head, list->tail, compareTime);
		else if (data->arg.sort_size)
			dll_bubble_sort(list->head, list->tail, compareSize);
		else
			dll_bubble_sort(list->head, list->tail, compareName);
	}

	if (data->arg.reverse)
		dll_revert(list);

	if (data->arg.recurisve)
		print_recursive(data, list);
	else {
		if (data->arg.long_format || !data->is_tty || data->arg.oneline)
			print_direct(data, list);
		else
			print_column(data, list);
	}

	// t_node *node = list->head;
	// t_ls *ls = node->content;
	// if (ls->format_info) {
	// 	printf("free format info\n");
	// 	freeFormatStruct(&ls->format_info);
	// 	ls->format_info = NULL;
	// }
	// print_column(data, list);
}
