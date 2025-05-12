/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/05/12 21:13:04 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void initArgStruct(t_arg *argStruct)
{
	argStruct->path = NULL;
	argStruct->all_path = NULL;
	argStruct->all = false;
	argStruct->reverse = false;
	argStruct->sort_time = false;
	argStruct->recurisve = false;
	argStruct->long_format = false;
	argStruct->extended_attributes = false;
}

void initFormatStruct(t_format *format)
{
	ft_memset(format, 0, sizeof(t_format));
}

void initInfoStruct(t_info *info)
{
	info->size_bytes = 0;
	info->group_name = NULL;
	info->user_name = NULL;
	info->last_mod = NULL;
	info->nlink = 0;
	ft_memset(info->perm, 0, sizeof(info->perm));
	ft_memset(info->sym_name, 0, sizeof(info->sym_name));

	info->nlink_len = 0;
	info->user_name_len = 0;
	info->group_name_len = 0;
	info->size_bytes_len = 0;
}