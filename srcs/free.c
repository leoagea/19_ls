/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:20:05 by lagea             #+#    #+#             */
/*   Updated: 2025/06/02 15:41:20 by lagea            ###   ########.fr       */
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
}

void freeArgStruct(t_arg *argList)
{
	if (argList->path) {
		freeStr(&argList->path);
	}
	if (argList->all_paths) {
		free2Array(argList->all_paths);
		argList->all_paths = NULL;
	}
	if (argList->input_list) {
		freeInputStruct(argList->input_list);
		argList->input_list = NULL;
	}
}

void freeLsNode(void *content)
{
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
	if (node->lower_name) {
		freeStr(&node->lower_name);
	}
	if (node->subdir) {
		if (node->subdir->head) {
			dll_free(node->subdir, freeLsNode);
			free(node->subdir);
			node->subdir = NULL;
		} else
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
	t_dll  *list = (t_dll *)content;
	t_node *node = list->head;
	while (node != NULL) {
		t_node *next = node->next;
		freeLsNode(node->content);
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
	dll_free(data->file_list, freeLsNode);
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

void freeInputStruct(t_input *start)
{
	while (start) {
		t_input *next = start->next;
		if (start->name)
			freeStr(&start->name);
		free(start);
		start = next;
	}
}