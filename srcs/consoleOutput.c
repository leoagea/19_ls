/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consoleOutput.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:44:36 by lagea             #+#    #+#             */
/*   Updated: 2025/06/09 16:27:29 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void sort_inside_dir(t_data *data, t_dll *list)
{
	if (!list || !list->head)
		return;

	if (!data->arg.not_sort) {
		if (data->arg.sort_time || (data->arg.access_time && !data->arg.long_format))
			dll_quick_sort(list, compareTime);
		else if (data->arg.access_time && data->arg.sort_time && data->arg.long_format)
			dll_quick_sort(list, compareTime);
		else if (data->arg.sort_size)
			dll_quick_sort(list, compareSize);
		else
			dll_quick_sort(list, compareName);
	}

	if (data->arg.reverse)
		dll_revert(list);
}

static void print_total_blocks(t_data *data, t_dll *list)
{
	int total_blocks = calculateTotalBlocks(list);

	if (data->arg.human_readable) {
		char *human_readable = get_human_readable_size(total_blocks * 1024);
		if (human_readable) {
			ft_printf(1, "%s %s\n", TOTAL_BLOCKS, human_readable);
			free(human_readable);
		} else {
			ft_printf(1, "%s %d\n", TOTAL_BLOCKS, total_blocks);
		}
	} else {
		ft_printf(1, "%s %d\n", TOTAL_BLOCKS, total_blocks);
	}
}

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
			ft_printf(1, "\t%s\t%d", attr->name, tmp);
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

		if (data->arg.slash && ls->type == DIRECTORY) {
			ft_printf(1, "/");
		}

		if (ls->xattr_list && ls->xattr_list->head) {
			ft_printf(1, "\n");
			print_xattr(ls->xattr_list);
		}

	} else {
		ft_printf(1, "%s%s%s", color, ls->format, reset);
		if (data->arg.slash && ls->type == DIRECTORY)
			ft_printf(1, "/");
	}
}

static void print_column(t_data *data, t_dll *list)
{
	// printf("print_column called\n");
	if (data->arg.block_size)
		print_total_blocks(data, list);
	t_ls **arr = list_to_stringarray(list);
	if (!arr) {
		return;
	}

	bool   is_block_size = data->arg.block_size;
	size_t arr_len = ft_arr_len((void **)arr);
	size_t columns = calculate_columns(arr, arr_len, data->w.ws_col, is_block_size);
	if (columns == 0) {
		free(arr);
		return;
	}
	size_t rows = (arr_len + columns - 1) / columns;

	size_t *col_widths = malloc(columns * sizeof(size_t));
	if (!col_widths) {
		free(arr);
		ft_printf(1, "\n");
		return;
	}

	size_t *siz_widths = NULL;
	if (is_block_size) {
		siz_widths = malloc(columns * sizeof(size_t));
		if (!siz_widths) {
			free(col_widths);
			free(arr);
			return;
		}
		ft_memset(siz_widths, 0, columns * sizeof(size_t));

		for (size_t col = 0; col < columns; col++) {
			siz_widths[col] = 0;
			for (size_t row = 0; row < rows; row++) {
				size_t idx;
				if (data->arg.horizontal)
					idx = row * columns + col;
				else
					idx = col * rows + row;
				if (idx < arr_len) {
					size_t len = arr[idx]->info->block_size_len;
					if (len > siz_widths[col])
						siz_widths[col] = len;
				}
			}
		}
	}

	for (size_t col = 0; col < columns; col++) {
		col_widths[col] = 0;
		for (size_t row = 0; row < rows; row++) {
			size_t idx;
			if (data->arg.horizontal)
				idx = row * columns + col;
			else
				idx = col * rows + row;
			if (idx < arr_len) {
				size_t len = strlen(arr[idx]->name);
				if (len > col_widths[col])
					col_widths[col] = len;
			}
		}
	}

	for (size_t row = 0; row < rows; row++) {
		for (size_t col = 0; col < columns; col++) {
			size_t idx;
			if (data->arg.horizontal)
				idx = row * columns + col;
			else
				idx = col * rows + row;
			if (idx < arr_len) {
				char *color = data->is_tty ? get_color_from_env(arr[idx], data) : "";
				char *reset = data->is_tty ? COLOR_RESET : "";
				int	  padding = 0;
				if (is_block_size) {
					padding = siz_widths[col] - arr[idx]->info->block_size_len;
					if (padding > 0) {
						addPadding(padding);
					}
					if (data->arg.block_size && !data->arg.human_readable)
						ft_printf(1, "%d ", arr[idx]->info->block_size);
					else if (data->arg.human_readable) {
						if (arr[idx]->type == DIRECTORY)
							ft_printf(1, "%s ", "0");
						else if (arr[idx]->type == REGFILE && arr[idx]->info->size_bytes == 0)
							ft_printf(1, "%s ", "0");
						else if (arr[idx]->type == REGFILE && arr[idx]->info->size_bytes > 0) {
							char *human_readable =
								get_human_readable_size(arr[idx]->info->size_bytes);
							ft_printf(1, "%s ", human_readable);
							free(human_readable);
						} else
							ft_printf(1, "%s ", "0");
					}
				}
				ft_printf(1, "%s%s%s", color, arr[idx]->name, reset);
				if (data->arg.slash && arr[idx]->type == DIRECTORY) {
					ft_printf(1, "/");
				}
				if (data->arg.comma && col < columns - 1) {
					ft_printf(1, ",");
				}
				ft_printf(1, "  ");
				padding = col_widths[col] - strlen(arr[idx]->name);
				if (padding > 0) {
					addPadding(padding);
				}
			}
		}
		ft_printf(1, "\n");
	}

	free(col_widths);
	free(arr);
	if (siz_widths) {
		free(siz_widths);
		siz_widths = NULL;
	}
}

static void handle_subdirs(t_data *data, t_dll *printsubdir)
{
	dll_quick_sort(printsubdir, compareSubdirName);
	if (data->arg.reverse)
		dll_revert(printsubdir);

	t_node *current = printsubdir->head;
	while (current) {
		t_subdir *tmp = current->content;
		if (tmp && tmp->subdir_list) {
			ft_printf(1, "\n%s:\n", tmp->path);
			sort_inside_dir(data, tmp->subdir_list);
			print_recursive(data, tmp->subdir_list);
		}
		current = current->next;
	}
}

static void print_direct(t_data *data, t_dll *list)
{
	// printf("print_direct called\n");
	if (data->arg.block_size || data->arg.long_format) {
		print_total_blocks(data, list);
	}

	size_t max_block_len = 0;
	if (data->arg.block_size) {
		t_node *tmp = list->head;
		while (tmp != NULL) {
			t_ls *ls = tmp->content;
			if (ls && ls->info) {
				size_t block_len = ft_strlen(ls->format_block);
				if (block_len > max_block_len) {
					max_block_len = block_len;
				}
			}
			tmp = tmp->next;
		}
	}

	t_node *node = list->head;
	while (node != NULL) {
		if (!node->content) {
			node = node->next;
			continue;
		}

		t_ls *ls = node->content;
		if (ls->format_block[0] != '\0') {
			size_t padding = ft_strlen(ls->format_block);
			while (padding < max_block_len) {
				ft_printf(1, " ");
				padding++;
			}
			ft_printf(1, "%s ", ls->format_block);
		}
		print_format(data, ls);
		ft_printf(1, "\n");
		node = node->next;
	}
}

static void print_comma(t_data *data, t_dll *list)
{
	if (data->arg.block_size) {
		print_total_blocks(data, list);
	}

	t_node *node = list->head;
	size_t	line_length = 0;
	while (node != NULL) {
		if (!node->content) {
			node = node->next;
			continue;
		}

		t_ls *ls = node->content;
		if (line_length + ft_strlen(ls->name) + 2 > data->w.ws_col) {
			ft_printf(1, "\n");
			line_length = 0;
		}
		if (data->arg.block_size)
			ft_printf(1, "%s", ls->format_block);
		print_format(data, ls);
		line_length += ft_strlen(ls->name) + 2;
		if (node->next)
			ft_printf(1, ", ");
		node = node->next;
	}
	ft_printf(1, "\n");
}

void print_recursive(t_data *data, t_dll *list)
{
	// printf("print_recursive called\n");
	t_dll *printsubdir = malloc(sizeof(t_dll));
	if (!printsubdir) {
		return;
	}
	dll_init(printsubdir);

	if (data->arg.long_format && data->is_tty) {
		print_total_blocks(data, list);

		t_node *node = list->head;
		while (node) {
			if (node->content) {
				t_ls *ls = node->content;
				print_format(data, ls);
				ft_printf(1, "\n");
			}
			node = node->next;
		}
	} else if (!data->is_tty || data->arg.oneline) {
		print_direct(data, list);
	} else {
		print_column(data, list);
	}

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

	handle_subdirs(data, printsubdir);

	dll_free(printsubdir, freeSubdir);
	free(printsubdir);
}

static void print_fileline(t_data *data, t_dll *list)
{
	t_node *node = list->head;
	while (node != NULL) {
		if (!node->content) {
			node = node->next;
			continue;
		}

		t_ls *ls = node->content;
		print_format(data, ls);
		ft_printf(1, "\n");
		node = node->next;
	}
}

void output(t_data *data, t_dll *list)
{
	sort_inside_dir(data, list);

	if (data->arg.recurisve)
		print_recursive(data, list);
	else {
		if (data->arg.long_format || !data->is_tty || data->arg.oneline)
			print_direct(data, list);
		else if (data->arg.comma)
			print_comma(data, list);
		else
			print_column(data, list);
	}
}

void outputListFiles(t_data *data, t_dll *list)
{
	sort_inside_dir(data, list);

	if (data->arg.long_format || !data->is_tty || data->arg.oneline)
		print_fileline(data, list);
	else {
		if (data->arg.comma)
			print_comma(data, list);
		else {
			print_column(data, list);
			ft_printf(1, "\n");
		}
	}
}