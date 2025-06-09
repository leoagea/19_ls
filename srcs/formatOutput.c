/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatOutput.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 00:09:36 by lagea             #+#    #+#             */
/*   Updated: 2025/06/09 16:26:54 by lagea            ###   ########.fr       */
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

void getFormatLen(t_ls *node, t_format **format)
{
	(*format)->max_group = MAX((*format)->max_group, node->info->group_name_len);
	(*format)->max_user = MAX((*format)->max_user, node->info->user_name_len);
	(*format)->max_link = MAX((*format)->max_link, node->info->nlink_len);
	(*format)->max_size_bytes = MAX((*format)->max_size_bytes, node->info->size_bytes_len);
	(*format)->max_name = MAX((*format)->max_name, node->info->name_len);
	(*format)->max_block_size = MAX((*format)->max_block_size, node->info->block_size_len);
	(*format)->max_uid = MAX((*format)->max_uid, node->info->user_id_len);
	(*format)->max_gid = MAX((*format)->max_gid, node->info->group_id_len);
}

static void fillInSpace(char *str, int max_len, int len)
{
	int i = -1;
	int spaces = max_len - len + 1;

	if (len == max_len) {
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

static void appendSize(t_arg arg, t_ls *node)
{
	if (node->type == BLKFILE || node->type == CHARFILE)
		appendStr(node->format, node->info->major);
	else {
		char *tmp = ft_itoa(node->info->size_bytes);
		if (!tmp) {
			tmp = ft_strdup("0");
		}
		char *append = NULL;
		if (arg.comma) {
			append = int_to_str_sep(node, tmp);
			if (!append) {
				append = ft_strdup("0");
			}
			appendStr(node->format, append);
			free(append);
		} else if (arg.human_readable) {
			append = get_human_readable_size(node->info->size_bytes);
			if (!append) {
				append = ft_strdup("0");
			}
			appendStr(node->format, append);
			if (append)
				free(append);
		} else
			appendStr(node->format, tmp);
		free(tmp);
	}
}

void formatLongFormat(t_arg arg, t_ls *node, t_format *format)
{
	if (arg.block_size && !arg.human_readable) {
		fillInSpace(node->format, format->max_block_size - 1, node->info->block_size_len);
		appendInt(node->format, node->info->block_size);
		appendChar(node->format, ' ');
	} else if (arg.block_size && arg.human_readable) {
		char *human_readable = get_human_readable_size(node->info->block_size * 1024);
		if (human_readable) {
			appendStr(node->format_block, human_readable);
			free(human_readable);
		} else {
			appendInt(node->format, node->info->block_size);
		}
	}

	appendType(node->format, node->type);

	appendStr(node->format, node->info->perm);
	if (!LINUX && node->info->perm[9] == '\0')
		appendChar(node->format, ' ');
	else if (LINUX && format->has_acl && node->info->perm[9] != ACL_CHAR)
		appendChar(node->format, ' ');
	fillInSpace(node->format, format->max_link, node->info->nlink_len);
	appendInt(node->format, node->info->nlink);

	if (!arg.no_name && !arg.id) {
		appendChar(node->format, ' ');
		appendStr(node->format, node->info->user_name);
		fillInSpace(node->format, format->max_user, node->info->user_name_len);
	} else if (arg.id) {
		fillInSpace(node->format, format->max_uid, node->info->user_id_len);
		appendInt(node->format, node->info->user_id);
	}
	// appendChar(node->format, ' ');

	if (arg.id) {
		fillInSpace(node->format, format->max_gid, node->info->group_id_len);
		appendInt(node->format, node->info->group_id);
		appendChar(node->format, ' ');
	} else {
		if (arg.no_name)
			appendChar(node->format, ' ');
		appendStr(node->format, node->info->group_name);
		fillInSpace(node->format, format->max_group, node->info->group_name_len);
	}

	fillInSpace(node->format, format->max_size_bytes - 1, node->info->size_bytes_len);
	appendSize(arg, node);
	appendChar(node->format, ' ');

	appendStr(node->format, node->info->time);
	appendChar(node->format, ' ');
}

void formatOther(t_arg arg, t_ls *node)
{
	if (arg.block_size) {
		if (arg.human_readable) {
			char *human_readable = get_human_readable_size(node->info->block_size * 1024);
			if (human_readable) {
				appendStr(node->format_block, human_readable);
				free(human_readable);
			} else {
				appendInt(node->format_block, node->info->block_size);
			}
		} else {
			appendInt(node->format_block, node->info->block_size);
		}
	}
	appendStr(node->format, node->name);
}

void formatOutput(t_format *format, t_ls *node, t_arg arg)
{
	if (!node) {
		return;
	}

	if (arg.long_format) {
		if (!node->info)
			return;
		formatLongFormat(arg, node, format);
	} else
		formatOther(arg, node);
}
