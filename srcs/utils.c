/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/05/13 23:04:06 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void usage(char invalidOption)
{
	ft_printf(2, "ls: invalid option -- %c\n", invalidOption);
	ft_printf(2, "usage: ls [-alrtR] [file ...]\n");
    ft_printf(2, "Try 'ls --help' for more information.\n");
}

void help(void)
{
    ft_printf(1, DESCIPTION);
    ft_printf(1, OPTIONS);
    ft_printf(1, AFLAG);
    ft_printf(1, RFLAG);
    ft_printf(1, TFLAG);
    ft_printf(1, LFLAG);
    ft_printf(1, EXIT);
}

char *extractTimeModified(struct stat info)
{
    time_t now = time(NULL);
    time_t mod_sec = info.st_mtime;
    time_t six_months = 15552000;
    char *full_time_str = ctime(&mod_sec);
    
    if (!full_time_str) {
        perror("ctime");
        return NULL;
    }

    char *time_str = malloc(13 * sizeof(char));
    if (!time_str)
        return NULL;
    ft_memset(time_str, 0, 13);
    
    if ((now - mod_sec > six_months) || (mod_sec > now)) {
        ft_memcpy(time_str, &full_time_str[4], 7);
        ft_memcpy(time_str + 7, &full_time_str[19], 5);
    } else {
        ft_memcpy(time_str, &full_time_str[4], 12);
        time_str[12] = '\0';
    }

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
    perm[9] = ' ';
    perm[10] = '\0';
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

int compareSize(void *a, void *b)
{
    t_ls *node_a = a;
    t_ls *node_b = b;
    if (node_a->info->size_bytes == node_b->info->size_bytes)
        return compareName(a, b);
    return node_b->info->size_bytes - node_a->info->size_bytes;
}

int calculateTotalBlocks(t_dll *list)
{
    int total_blocks = 0;
    t_node *curr = list->head;
    
    while (curr != NULL) {
        t_ls *node = curr->content;
        total_blocks += node->info->block_size;
        curr = curr->next;
    }

    return CALC_BLOCKS(total_blocks);
}

t_ls *mallocLs(t_format *format)
{
    t_ls *node;
    node = malloc(sizeof(t_ls));
    if (!node)
        return NULL;
    ft_memset(node, 0, sizeof(t_ls));
    
    node->subdir = NULL;
    node->info = NULL;
    node->format_info = format;
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

char *get_color_for_file(t_ls *ls)
{
    switch (ls->type) {
        case DIRECTORY:
            return COLOR_DIR;
        case LINK:
            return COLOR_LINK;
        case SOCKET:
            return COLOR_SOCKET;
        case FIFO:
            return COLOR_PIPE;
        case BLKFILE:
            return COLOR_BLOCK;
        case CHARFILE:
            return COLOR_CHAR;
        case REGFILE:
            if (ls->info && ls->info->perm[2] == 'x')
                return COLOR_EXEC;
    }
    return COLOR_RESET;
}

char *int_to_str_sep(t_ls *node, char *num)
{   
    size_t thousands = node->info->size_thousands;
    char *str = malloc(12 + thousands);
    if (!str)
        return NULL;

    ft_memset(str, 0, 12 + node->info->size_thousands);
    
    struct lconv *loc;
    loc = localeconv();
    char sep = loc->thousands_sep[0] ? loc->thousands_sep[0] : ',';

    int num_len = ft_strlen(num);
    int sep_count = node->info->size_thousands;
    int total_len = num_len + sep_count + 1;
    
    int i = num_len - 1;
    int j = total_len - 2;
    int count = 0;

    while (i >= 0 && j >= 0) {
        str[j] = num[i];
        count++;
        i--;
        j--;
        
        if (i >= 0 && count % 3 == 0 && j >= 0) {
            str[j] = sep;
            j--;
        }
    }
    return str;
}