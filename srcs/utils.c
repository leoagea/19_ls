/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/06/02 16:39:15 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void usage(char invalidOption)
{
	ft_printf(2, "ls: invalid option -- %c\n", invalidOption);
	ft_printf(2, "usage: ls [-alrtR] [file ...]\n");
	ft_printf(2, "Try 'ls --help' for more information.\n");
}

void help(void)
{
	ft_printf(1, DESCIPTION);
	ft_printf(1, OPTIONS);
	ft_printf(1, AFLAG);
	ft_printf(1, RFLAG);
	ft_printf(1, TFLAG);
	ft_printf(1, LFLAG);
	ft_printf(1, EXTFLAG);
	ft_printf(1, SIZEFLAG);
	ft_printf(1, FFLAG);
	ft_printf(1, GFLAG);
	ft_printf(1, SLASHFLAG);
	ft_printf(1, ONEEFLAG);
	ft_printf(1, COMMAFLAG);
	ft_printf(1, MFLAG);
	ft_printf(1, UFLAG);
	ft_printf(1, DFLAG);
	ft_printf(1, HFLAG);
	ft_printf(1, EXIT);
}

int calculateTotalBlocks(t_dll *list)
{
	int		total_blocks = 0;
	t_node *curr = list->head;

	while (curr != NULL) {
		t_ls *node = curr->content;
		total_blocks += (node->info) ? node->info->block_size : 0;
		curr = curr->next;
	}

	return total_blocks;
}

char *get_color_for_file(t_ls *ls)
{
	switch (ls->type) {
		case DIRECTORY:
			return COLOR_DIR;
		case LINK:
			return COLOR_LINK;
		case SOCKET:
			return COLOR_SOCKET;
		case FIFO:
			return COLOR_PIPE;
		case BLKFILE:
			return COLOR_BLOCK;
		case CHARFILE:
			return COLOR_CHAR;
		case REGFILE:
			if (ls->info && ls->info->perm[2] == 'x')
				return COLOR_EXEC;
	}
	return COLOR_RESET;
}

char *int_to_str_sep(t_ls *node, char *num)
{
	size_t thousands = node->info->size_thousands;
	char  *str = malloc(12 + thousands);
	if (!str)
		return NULL;

	ft_memset(str, 0, 12 + node->info->size_thousands);

	struct lconv *loc;
	loc = localeconv();
	char sep = loc->thousands_sep[0] ? loc->thousands_sep[0] : ',';

	int num_len = ft_strlen(num);
	int sep_count = node->info->size_thousands;
	int total_len = num_len + sep_count + 1;

	int i = num_len - 1;
	int j = total_len - 2;
	int count = 0;

	while (i >= 0 && j >= 0) {
		str[j] = num[i];
		count++;
		i--;
		j--;

		if (i >= 0 && count % 3 == 0 && j >= 0) {
			str[j] = sep;
			j--;
		}
	}
	return str;
}

char *string_to_lower(char *str)
{
	size_t len = ft_strlen(str);
	char  *lower_str = malloc(len + 1);
	if (!lower_str)
		return NULL;

	for (int i = 0; str[i]; i++) {
		lower_str[i] = ft_tolower(str[i]);
	}
	lower_str[len] = '\0';

	return lower_str;
}

int get_max_len(t_dll *list)
{
	size_t	max_len = 0;
	t_node *node = list->head;

	while (node != NULL) {
		t_ls *ls = node->content;
		if (ls && ls->info && ls->info->name_len > max_len) {
			max_len = ls->info->name_len;
		}
		node = node->next;
	}
	return max_len;
}

t_ls **list_to_stringarray(t_dll *list)
{
	size_t size = dll_size(list);
	t_ls **arr = malloc((size + 1) * sizeof(t_ls *));
	if (!arr)
		return NULL;

	t_node *node = list->head;
	size_t	i = 0;

	while (node != NULL) {
		t_ls *ls = node->content;
		if (ls) {
			arr[i] = ls;
			i++;
		}
		node = node->next;
	}
	arr[i] = NULL;
	return arr;
}

size_t calculate_columns(t_ls **files, size_t arr_len, size_t screen_width, bool is_block_size)
{
	size_t count = arr_len;
	if (count == 0)
		return 0;

	size_t max_filename_len = 0;
	size_t total_len = 0;
	for (size_t i = 0; i < count; i++) {
		size_t name_len = strlen(files[i]->name);
		size_t len = name_len;
		if (is_block_size) {
			len += files[i]->info->block_size_len + 2;
		}
		total_len += len + 2;
		if (len > max_filename_len)
			max_filename_len = len;
	}

	if (total_len < screen_width) {
		return count;
	}
	size_t col_width = max_filename_len + 2;

	size_t max_cols = screen_width / col_width;
	if (max_cols == 0)
		max_cols = 1;
	if (max_cols > count) {
		max_cols = count;
	}

	size_t optimal_cols = max_cols;
	return optimal_cols;
}

void addPadding(size_t len)
{
	for (size_t i = 0; i < len; i++) {
		ft_printf(1, " ");
	}
}

char *get_human_readable_size(size_t size)
{
	char  *units[] = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
	int	   unit_index = 0;
	double human_size = (double)size;

	while (human_size >= 1024 && unit_index < 8) {
		human_size /= 1024;
		unit_index++;
	}

	char *result = malloc(32);
	if (!result)
		return NULL;
	ft_memset(result, 0, 32);

	if (unit_index == 0) {
		char *num = ft_itoa(size);
		if (!num) {
			free(result);
			return NULL;
		}
		ft_strlcpy(result, num, 32);
		free(num);
	} else if (human_size < 10) {
		human_size = (int)(human_size * 10 + 0.5) / 10.0;

		int	  int_part = (int)human_size;
		char *int_str = ft_itoa(int_part);
		if (!int_str) {
			free(result);
			return NULL;
		}
		int	  decimal = (int)((human_size - int_part) * 10 + 0.5);
		char *dec_str = ft_itoa(decimal);
		if (!dec_str) {
			free(int_str);
			free(result);
			return NULL;
		}
		ft_strlcpy(result, int_str, 32);
		ft_strlcat(result, ".", 32);
		ft_strlcat(result, dec_str, 32);
		ft_strlcat(result, units[unit_index], 32);
		free(int_str);
		free(dec_str);
	} else {
		human_size = (int)(human_size + 0.5);
		char *num = ft_itoa((int)human_size);
		if (!num) {
			free(result);
			return NULL;
		}
		ft_strlcpy(result, num, 32);
		ft_strlcat(result, units[unit_index], 32);
		free(num);
	}

	return result;
}
