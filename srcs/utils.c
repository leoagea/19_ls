/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/03/25 01:11:56 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void usage(char invalidOption)
{
	ft_printf(2, "ls: invalid option -- %c\n", invalidOption);
	ft_printf(2, "usage: ls [-alrtR] [file ...]\n");	
}

void freeArgStruct(t_arg *argList)
{
	if (argList->path){
		free(argList->path);
		argList->path = NULL;
	}
}

t_ls_node *newLsNode(struct dirent *entry)
{
	t_ls_node *node = malloc(sizeof(t_ls_node));
	if (!node)
		return NULL;

	struct stat *info = NULL;
	stat(node->name, info);
	node->info = info;
	
	node->entry = entry;
	node->name = entry->d_name;
	
	if (entry->d_type == LINK){
		node->symbolic = true;
		readlink(node->name, node->sym_name, 256);
	}
	else
		node->symbolic = false;
	
	return node;
}