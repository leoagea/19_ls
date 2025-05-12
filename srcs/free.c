/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:20:05 by lagea             #+#    #+#             */
/*   Updated: 2025/05/12 21:12:20 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void free2Array(char **array)
{
    int i = 0;
    
    while (array[i]){
        free(array[i]);
        array[i] = NULL;
        i++;
    }
    free(array);
    array = NULL;
}

static void freeInfoStruct(t_info *info)
{
    if (info->group_name)
        freeStr(&info->group_name);
    if (info->user_name)
        freeStr(&info->user_name);
    if (info->last_mod)
        freeStr(&info->last_mod);
}

void freeArgStruct(t_arg *argList)
{
	if (argList->path){
		freeStr(&argList->path);
	}
    if (argList->all_path){
        free2Array(argList->all_path);
        argList->all_path = NULL;
    }
}

void freeLsNode(void *content)
{
    t_ls *node = (t_ls *)content;
    
    if (node->info){
        freeInfoStruct(node->info);
        free(node->info);
        node->info = NULL;
    }
    if (node->name){
        freeStr(&node->name);
    }
    if (node->relative_path){
        freeStr(&node->relative_path);
    }
    if (node->subdir && node->subdir->head){
        printf("free subdir\n");
        dll_free(node->subdir, freeLsNode);
        free(node->subdir);
        node->subdir = NULL;
    }
}

void freeStr(char **str)
{
    if (*str){
        free(*str);
        *str = NULL;
    }
}

void freeVoid(void **content)
{
    if (*content){
        free(*content);
        *content = NULL;
    }
}

void freeList(void *content)
{
    printf("free list\n");
    t_dll *list = (t_dll *)content;
    t_node *node = list->head;
    while (node != NULL) {
        t_node *next = node->next;
        freeLsNode(node->content);
        free(node->content);
        free(node);
        node = next;
    }
    list->head = NULL;
    list->tail = NULL;
    free(list);
}

void freeFormatStruct(t_format **format)
{
    if (*format){
        free(*format);
        *format = NULL;
    }
}

void freeXattr(void *content)
{
    t_xattr *xattr = (t_xattr *)content;
    if (xattr->name)
        freeStr(&xattr->name);
    if (xattr->value)
        freeVoid(&xattr->value);
}