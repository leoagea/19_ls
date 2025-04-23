/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/04/23 17:05:52 by lagea            ###   ########.fr       */
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

char *extractTimeModified(struct stat info)
{
	time_t mod_sec = info.st_mtime;
    char *full_time_str = ctime(&mod_sec);
    if (!full_time_str) {
        perror("ctime");
        return NULL;
    }

    char *time_str = malloc(13 * sizeof(char)); // 12 + 1 for '\0'
    ft_memcpy(time_str, &full_time_str[4], 12);
    time_str[12] = '\0';

    return time_str;
}

void extractPerm(char *perm, int mode)
{
    perm[0] = (mode & S_IRUSR) ? 'r' : '-';
    perm[1] = (mode & S_IWUSR) ? 'w' : '-';
    perm[2] = (mode & S_IXUSR) ? 'x' : '-';
    perm[3] = (mode & S_IRGRP) ? 'r' : '-';
    perm[4] = (mode & S_IWGRP) ? 'w' : '-';
    perm[5] = (mode & S_IXGRP) ? 'x' : '-';
    perm[6] = (mode & S_IROTH) ? 'r' : '-';
    perm[7] = (mode & S_IWOTH) ? 'w' : '-';
    perm[8] = (mode & S_IXOTH) ? 'x' : '-';
    perm[9] = '\0';
}

int compareName(void *a, void *b)
{
    t_ls *node_a = a;
    t_ls *node_b = b;
    return ft_strncmp(node_a->name, node_b->name, INT_MAX);
}

int compareTime(void *a, void *b)
{
    t_ls *node_a = a;
    t_ls *node_b = b;
    int cmp = ft_strncmp(node_a->info->last_mod, node_b->info->last_mod, INT_MAX);
    if (cmp == 0)
        return compareName(a, b);
    return -ft_strncmp(node_a->info->last_mod, node_b->info->last_mod, INT_MAX);
}

int compareSubdirName(void *a, void *b)
{
    t_subdir *node_a = a;
    t_subdir *node_b = b;
    return ft_strncmp(node_a->name, node_b->name, INT_MAX);
}

int calculateTotalBlocks(t_dll *list)
{
    int total_blocks = 0;
    t_node *curr = list->head;
    
    while (curr != NULL) {
        struct stat info;
        t_ls *node = (t_ls *)curr->content;
        if (lstat(node->relative_path, &info) == 0)
            total_blocks += (int)info.st_blocks;
        curr = curr->next;
    }

    return total_blocks / 2;
}

t_ls *mallocLs(t_format *format)
{
    t_ls *node;
    node = malloc(sizeof(t_ls));
    if (!node)
        return NULL;
    ft_memset(node, 0, sizeof(t_ls));
    
    node->subdir = NULL;

    node->format_info = format;
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