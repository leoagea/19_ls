/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatOutput.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 00:09:36 by lagea             #+#    #+#             */
/*   Updated: 2025/04/21 18:42:03 by lagea            ###   ########.fr       */
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

void getFormatLen(t_ls *node, t_format *format)
{
	format->max_group = MAX(format->max_group, node->info->group_name_len);
	format->max_user = MAX(format->max_user, node->info->user_name_len);
	format->max_link = MAX(format->max_link, node->info->nlink_len);
	format->max_size_bytes = MAX(format->max_size_bytes, node->info->size_bytes_len);
}

static void fillInSpace(t_ls *node, int max_len, int len)
{
	int i = 0;
	
	appendChar(node->format, ' ');
	while (++i <= max_len - len)
		appendChar(node->format, ' ');
}

void formatLongFormat(t_ls *node, t_format *format)
{
	if (node->type == DIRECTORY)
		appendChar(node->format, 'd');
	else if (node->type == FILE)
		appendChar(node->format, '-');
	else if (node->type == LINK)
		appendChar(node->format, 'l');
	else
		appendChar(node->format, 'u');
	
	appendStr(node->format, node->info->perm);
	fillInSpace(node, format->max_link, node->info->nlink_len);
	char *tmp = ft_itoa(node->info->nlink);
	appendStr(node->format, tmp);
	free(tmp);
	
	fillInSpace(node, format->max_user, node->info->user_name_len);
	appendStr(node->format, node->info->user_name);
	
	fillInSpace(node, format->max_group, node->info->group_name_len);
	appendStr(node->format, node->info->group_name);
	
	fillInSpace(node, format->max_size_bytes, node->info->size_bytes_len);
	tmp = ft_itoa(node->info->size_bytes);
	appendStr(node->format, tmp);
	free(tmp);
	appendChar(node->format, ' ');
	
	appendStr(node->format, node->info->last_mod);
	appendChar(node->format, ' ');
	
	appendStr(node->format, node->name);
	if (node->is_symbolic){
    	appendStr(node->format, " -> ");
		appendStr(node->format, node->info->sym_name);
	}
}

void formatOther(t_ls*node)
{
	appendStr(node->format, node->name);
}

void formatOutput(t_ls *node, t_arg arg)
{
	if (arg.long_format)
	{
		// printFormatStruct(node->format_info);
		if (node->format_info == NULL){
			printf("format_info null\n");
			return;
		}
		formatLongFormat(node, node->format_info);
	}
	else
		formatOther(node);
}
