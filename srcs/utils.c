/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/03/25 01:48:28 by lagea            ###   ########.fr       */
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

	struct stat info;
	stat(node->name, &info);
	node->info = &info;
	
	node->entry = entry;
	node->name = entry->d_name;
	node->last_mod = extractTimeModified(info);
	
	if (entry->d_type == LINK){
		node->symbolic = true;
		readlink(node->name, node->sym_name, 256);
	}
	else
		node->symbolic = false;
	
	return node;
}

char *extractTimeModified(struct stat info)
{
	time_t mod_sec = info.st_mtimespec.tv_sec;
    char *full_time_str = ctime(&mod_sec);
	printf("full time: %s\n",full_time_str);
    if (!full_time_str) {
        perror("ctime");
        return NULL;
    }

    char *time_str = malloc(13 * sizeof(char)); // 12 + 1 for '\0'
    memcpy(time_str, &full_time_str[4], 12);
    time_str[12] = '\0';

    return time_str;
}