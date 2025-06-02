/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/06/02 16:46:21 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void initArgStruct(t_arg *argStruct)
{
	ft_memset(argStruct, 0, sizeof(t_arg));
	argStruct->path = NULL;
	argStruct->all_paths = NULL;
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
	ft_memset(info->time, 0, sizeof(info->time));
	info->group_name = NULL;
	info->user_name = NULL;
	// info->last_mod = NULL;
}

t_ls *mallocLs(void)
{
	t_ls *node;
	node = malloc(sizeof(t_ls));
	if (!node)
		return NULL;
	ft_memset(node, 0, sizeof(t_ls));

	node->subdir = NULL;
	node->info = NULL;
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

t_input *mallocInput(void)
{
	t_input *node = malloc(sizeof(t_input));
	if (!node) 
		return NULL;
	node->name = NULL;
	node->type = 0;
	node->next = NULL;
	return node;
}