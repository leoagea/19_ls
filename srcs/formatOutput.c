/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formatOutput.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 00:09:36 by lagea             #+#    #+#             */
/*   Updated: 2025/03/27 16:16:16 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void appendStr(char *str, char *append)
{
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

void formatLongFormat(t_ls_node *node)
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
	appendChar(node->format, ' ');
	char *tmp = ft_itoa(node->nlink);
	appendStr(node->format, tmp);
	free(tmp);
	appendChar(node->format, ' ');
	appendStr(node->format, node->user_name);
	appendChar(node->format, ' ');
	appendStr(node->format, node->group_name);
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

void formatOutput(t_ls_node *node, t_arg arg)
{
	if (arg.long_format)
	{
		formatLongFormat(node);
	}
	else
		formatOther(node);
}
