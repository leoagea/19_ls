/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:20:05 by lagea             #+#    #+#             */
/*   Updated: 2025/05/26 15:07:34 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void free2Array(char **array)
{
	int i = 0;

	while (array[i]) {
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
	// if (info->time)
	// 	freeStr(&info->time);
}

void freeArgStruct(t_arg *argList)
{
	if (argList->path) {
		freeStr(&argList->path);
	}
	if (argList->all_path) {
		free2Array(argList->all_path);
		argList->all_path = NULL;
	}
}

void freeLsNode(void *content)
{
	// printf("free ls node\n");
	t_ls *node = (t_ls *)content;

	if (node->info) {
		freeInfoStruct(node->info);
		free(node->info);
		node->info = NULL;
	}
	if (node->name) {
		freeStr(&node->name);
	}
	if (node->relative_path) {
		freeStr(&node->relative_path);
	}
	if (node->lower_name){
		freeStr(&node->lower_name);
	}
	if (node->subdir) {
		if (node->subdir->head) {
			// t_node *sub = node->subdir->head;
			// t_ls *tmp = sub->content;
			// printf("free subdir %s\n", tmp->relative_path);
			// if (tmp->format_info) {
			// 	// printf("free format subdir\n");
			// 	freeFormatStruct(&tmp->format_info);
			// }
			dll_free(node->subdir, freeLsNode);
			free(node->subdir);
			node->subdir = NULL;
		}
		else
			free(node->subdir);
	}
	free(node);
}

void freeStr(char **str)
{
	if (*str) {
		free(*str);
		*str = NULL;
	}
}

void freeVoid(void **content)
{
	if (*content) {
		free(*content);
		*content = NULL;
	}
}

void freeList(void *content)
{
	// printf("free list\n");
	t_dll  *list = (t_dll *)content;
	t_node *node = list->head;
	// t_ls *ls = node->content;
	// if (ls->format_info){
	// 	// printf("free format\n");
	// 	freeFormatStruct(&ls->format_info);
	// }
	while (node != NULL) {
		t_node *next = node->next;
		freeLsNode(node->content);
		// free(node->content);
		free(node);
		node = next;
	}
	list->head = NULL;
	list->tail = NULL;
	free(list);
}

void freeFormatStruct(t_format **format)
{
	if (*format) {
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

void freeColorMap(t_color_map *colors)
{
	if (colors->di)
		freeStr(&colors->di);
	if (colors->ln)
		freeStr(&colors->ln);
	if (colors->so)
		freeStr(&colors->so);
	if (colors->pi)
		freeStr(&colors->pi);
	if (colors->ex)
		freeStr(&colors->ex);
	if (colors->bd)
		freeStr(&colors->bd);
	if (colors->cd)
		freeStr(&colors->cd);
}

void freeAll(t_data *data)
{
	freeArgStruct(&data->arg);
	freeColorMap(&data->colors);
	dll_free(data->list, freeList);
}

void freeSubdir(void *content)
{
	t_subdir *subdir = (t_subdir *)content;
	if (subdir->name)
		freeStr(&subdir->name);
	if (subdir->path)
		freeStr(&subdir->path);
	if (subdir->lower_name)
		freeStr(&subdir->lower_name);
	free(subdir);
}