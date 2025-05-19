/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:06:09 by lagea             #+#    #+#             */
/*   Updated: 2025/05/19 17:11:25 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int compareName(void *a, void *b)
{
	t_ls *node_a = a;
	t_ls *node_b = b;
	return ft_strncmp(node_a->lower_name, node_b->lower_name, INT_MAX);
}

int compareTime(void *a, void *b)
{
	t_ls *node_a = a;
	t_ls *node_b = b;
	
    if (node_a->info->last_mod_time > node_b->info->last_mod_time){
        return -1;
    }
    if (node_a->info->last_mod_time < node_b->info->last_mod_time){
        return 1;
    }
        
	return -compareName(a, b);
}

int compareSubdirName(void *a, void *b)
{
	t_subdir *node_a = a;
	t_subdir *node_b = b;
	return ft_strncmp(node_a->name, node_b->name, INT_MAX);
}

int compareSize(void *a, void *b)
{
	t_ls *node_a = a;
	t_ls *node_b = b;
	if (node_a->info->size_bytes == node_b->info->size_bytes)
		return compareName(a, b);
	return node_b->info->size_bytes - node_a->info->size_bytes;
}
