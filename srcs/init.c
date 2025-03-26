/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/03/26 17:29:41 by lagea            ###   ########.fr       */
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
	node->name = NULL;
	node->info = NULL;
	node->entry = NULL;
	node->last_mod = NULL;
	node->user_name = NULL;
	node->group_name = NULL;
	node->relative_path = NULL;
	node->size_bytes = 0;
	node->type = UNKNOWN;
	node->symbolic = false;
	ft_memset(node->sym_name, 0, sizeof(node->sym_name));
}
