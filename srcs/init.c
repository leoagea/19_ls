/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/03/27 00:32:20 by lagea            ###   ########.fr       */
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
}

void initLsNode(t_ls_node *node)
{
	node->relative_path = NULL;
	node->type = UNKNOWN;
	node->name = NULL;
	node->symbolic = false;
	ft_memset(node->format, 0, sizeof(node->format));
	
	node->size_bytes = 0;
	ft_memset(node->sym_name, 0, sizeof(node->sym_name));
	node->group_name = NULL;
	node->user_name = NULL;
	ft_memset(node->perm, 0, sizeof(node->perm));
	node->last_mod = NULL;
	node->nlink = 0;
	
	node->entry = NULL;
	node->info = NULL;
}
