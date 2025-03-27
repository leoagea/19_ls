/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieveInfo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:18:08 by lagea             #+#    #+#             */
/*   Updated: 2025/03/27 17:27:51 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

// int retrieveAllInfo(t_ls_node *node, t_arg arg, char *path, struct dirent *entry)
// {
    
// }

int retrieveSymInfo(t_ls_node *node, t_arg arg)
{
    size_t size = sizeof(node->sym_name);
	
	node->symbolic = true;
	// if (lstat(entry->d_name, &info) == -1)
	// 	return (printf("lstat failed\n"), NULL);
	if (arg.long_format){
		if (readlink(node->relative_path, node->sym_name, size) == -1)
			return (printf("readlink failed\n"), 0);
		node->size_bytes = ft_strlen(node->sym_name);
	}
	return 1;
}