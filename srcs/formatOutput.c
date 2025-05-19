/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatOutput.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 00:09:36 by lagea             #+#    #+#             */
/*   Updated: 2025/05/19 16:47:30 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void appendStr(char *str, char *append)
{
	if (!str || !append)
		return;
	while (*str)
		str++;
	while (*append)
		*str++ = *append++;
}

void appendChar(char *str, char c)
{
	if (!str)
		return;
	while (*str)
		str++;
	*str = c;
}

void appendInt(char *str, int num)
{
	if (!str)
		return;
	char *tmp = ft_itoa(num);
	appendStr(str, tmp);
	free(tmp);
}

void getFormatLen(t_ls *node, t_format *format)
{
	format->max_group = MAX(format->max_group, node->info->group_name_len);
	format->max_user = MAX(format->max_user, node->info->user_name_len);
	format->max_link = MAX(format->max_link, node->info->nlink_len);
	format->max_size_bytes =
		MAX(format->max_size_bytes, node->info->size_bytes_len);
	// printf("name len: %zu\n", node->info->name_len);
	format->max_name = MAX(format->max_name, node->info->name_len);
	format->max_block_size =
		MAX(format->max_block_size, node->info->block_size_len);
	format->max_uid = MAX(format->max_uid, node->info->user_id_len);
	format->max_gid = MAX(format->max_gid, node->info->group_id_len);
}

static void fillInSpace(char *str, int max_len, int len)
{
	int i = -1;
	int spaces = max_len - len + 1;

	if (len == max_len ){
		appendChar(str, ' ');
		return;
	}
	while (++i < spaces)
		appendChar(str, ' ');
}

static void appendType(char *format, int type)
{
	switch (type) {
	case DIRECTORY:
		appendChar(format, 'd');
		break;
	case REGFILE:
		appendChar(format, '-');
		break;
	case LINK:
		appendChar(format, 'l');
		break;
	case FIFO:
		appendChar(format, 'p');
		break;
	case SOCKET:
		appendChar(format, 's');
		break;
	case CFS:
		appendChar(format, 'n');
		break;
	case CHARFILE:
		appendChar(format, 'c');
		break;
	case BLKFILE:
		appendChar(format, 'b');
		break;
	default:
		appendChar(format, 'u');
		break;
	}
}

void formatLongFormat(t_arg arg, t_ls *node, t_format *format)
{
	if (arg.block_size) {
		fillInSpace(node->format, format->max_block_size, node->info->block_size_len);
		appendInt(node->format, node->info->block_size);
		appendChar(node->format, ' ');
	}
	appendType(node->format, node->type);

	appendStr(node->format, node->info->perm);
	fillInSpace(node->format, format->max_link, node->info->nlink_len);
	appendInt(node->format, node->info->nlink);

	if (!arg.no_name && !arg.id) {
		fillInSpace(node->format, format->max_user, node->info->user_name_len);
		appendStr(node->format, node->info->user_name);
	} else if (arg.id) {
		fillInSpace(node->format, format->max_uid, node->info->user_id_len);
		appendInt(node->format, node->info->user_id);
	}

	if (arg.id) {
		fillInSpace(node->format, format->max_gid, node->info->group_id_len);
		appendInt(node->format, node->info->group_id);
	} else {
		fillInSpace(node->format, format->max_group, node->info->group_name_len);
		appendStr(node->format, node->info->group_name);
	}

	fillInSpace(node->format, format->max_size_bytes, node->info->size_bytes_len);
	if (node->type == BLKFILE || node->type == CHARFILE)
		appendStr(node->format, node->info->major);
	else{
		char *tmp = ft_itoa(node->info->size_bytes);
		char *append = NULL;
		if (arg.comma) {
			append = int_to_str_sep(node, tmp);
			appendStr(node->format, append);
			free(append);
		} else
			appendStr(node->format, tmp);
		free(tmp);
	}
	appendChar(node->format, ' ');

	appendStr(node->format, node->info->last_mod);
	appendChar(node->format, ' ');

	// appendStr(node->format, node->name);
	// if (node->is_symbolic){
	// 	appendStr(node->format, " -> ");
	// 	appendStr(node->format, node->info->sym_name);
	// }
}

void formatOther(t_arg arg, t_ls *node)
{
	if (arg.block_size) {
		fillInSpace(node->format_block, node->format_info->max_block_size,
					node->info->block_size_len);
		appendInt(node->format_block, node->info->block_size);
		appendChar(node->format_block, ' ');
	}
	appendStr(node->format, node->name);

	int space = node->format_info->max_name - node->info->name_len + 1;
	for (int i = 0; i < space; i++)
		appendChar(node->format, ' ');
}

void formatOutput(t_ls *node, t_arg arg)
{
	if (!node) {
		printf("Error: Null node passed to formatOutput\n");
		return;
	}

	if (arg.long_format) {
		// debug_printFormatStruct(node->format_info);
		if (!node->format_info) {
			printf("Error: Null format_info in node passed to "
				   "formatOutput\n");
			return;
		}
		if (!node->info) {
			printf("Error: Null info in node passed to "
				   "formatOutput\n");
			return;
		}
		formatLongFormat(arg, node, node->format_info);
	} else
		formatOther(arg, node);
}
