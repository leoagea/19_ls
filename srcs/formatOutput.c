/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatOutput.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 00:09:36 by lagea             #+#    #+#             */
/*   Updated: 2025/04/18 17:20:35 by lagea            ###   ########.fr       */
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
	while (*str)
		str++;
	*str = c;
}

void getFormatLen(t_ls *node, t_format *format)
{
	format->max_group = MAX(format->max_group, node->group_name_len);
	format->max_user = MAX(format->max_user, node->user_name_len);
	format->max_link = MAX(format->max_link, node->nlink_len);
	format->max_size_bytes = MAX(format->max_size_bytes, node->size_bytes_len);
}

static void fillInSpace(t_ls_node *node, t_format *format)
{
	int i = 0;
	
	appendChar(node->format, ' ');
	while (++i <= format->max_link - node->nlink_len)
		appendChar(node->format, ' ');
}

void formatLongFormat(t_ls_node *node, t_format *format)
{
	if (node->type == DIRECTORY)
		appendChar(node->format, 'd');
	else if (node->type == FILE)
		appendChar(node->format, '-');
	else if (node->type == LINK)
		appendChar(node->format, 'l');
	else
		appendChar(node->format, 'u');
		
	appendStr(node->format, node->perm);
	
	fillInSpace(node, format);
	appendChar(node->format, ' ');
	char *tmp = ft_itoa(node->nlink);
	appendStr(node->format, tmp);
	free(tmp);
	
	fillInSpace(node, format);
	appendStr(node->format, node->user_name);
	
	fillInSpace(node, format);
	appendChar(node->format, ' ');
	appendStr(node->format, node->group_name);
	
	fillInSpace(node, format);
	appendChar(node->format, ' ');
	tmp = ft_itoa(node->size_bytes);
	appendStr(node->format, tmp);
	free(tmp);
	appendChar(node->format, ' ');
	
	appendStr(node->format, node->last_mod);
	appendChar(node->format, ' ');
	
	appendStr(node->format, node->name);
	if (node->symbolic){
    	appendStr(node->format, " -> ");
		appendStr(node->format, node->sym_name);
	}
}

void formatOther(t_ls_node *node)
{
	appendStr(node->format, node->name);
}

void formatOutput(t_ls_node *node, t_arg arg, t_format *format)
{
	if (arg.long_format)
	{
		formatLongFormat(node, format);
	}
	else
		formatOther(node);
}
