/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/05/13 00:57:07 by lagea            ###   ########.fr       */
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