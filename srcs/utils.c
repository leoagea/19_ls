/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/05/19 16:11:50 by lagea            ###   ########.fr       */
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
	ft_printf(1, EXIT);
}

int calculateTotalBlocks(t_dll *list)
{
	int		total_blocks = 0;
	t_node *curr = list->head;

	while (curr != NULL) {
		t_ls *node = curr->content;
		total_blocks += node->info->block_size;
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
	char *lower_str = malloc(len + 1);
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
	size_t max_len = 0;
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