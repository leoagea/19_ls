/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/05/19 16:05:42 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void initArgStruct(t_arg *argStruct)
{
	ft_memset(argStruct, 0, sizeof(t_arg));
	argStruct->path = NULL;
	argStruct->all_path = NULL;
}

void initFormatStruct(t_format *format)
{
	ft_memset(format, 0, sizeof(t_format));
}

void initInfoStruct(t_info *info)
{
	ft_memset(info, 0, sizeof(t_info));
	ft_memset(info->perm, 0, sizeof(info->perm));
	ft_memset(info->sym_name, 0, sizeof(info->sym_name));
	ft_memset(info->major, 0, sizeof(info->major));
	info->group_name = NULL;
	info->user_name = NULL;
	info->last_mod = NULL;
}

t_ls *mallocLs(t_format *format)
{
	t_ls *node;
	node = malloc(sizeof(t_ls));
	if (!node)
		return NULL;
	ft_memset(node, 0, sizeof(t_ls));

	node->subdir = NULL;
	node->info = NULL;
	node->format_info = format;
	node->xattr_list = NULL;
	return node;
}

t_subdir *mallocSubdir(void)
{
	t_subdir *subdir = malloc(sizeof(t_subdir));
	if (!subdir)
		return NULL;
	subdir->name = NULL;
	subdir->path = NULL;
	subdir->subdir_list = NULL;
	return subdir;
}