/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:06:09 by lagea             #+#    #+#             */
/*   Updated: 2025/06/06 13:16:59 by lagea            ###   ########.fr       */
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

	if (node_a->info->time_info != node_b->info->time_info) {
		return (node_b->info->time_info - node_a->info->time_info);
	}

	if (node_a->info->time_nsec != node_b->info->time_nsec) {
		return (node_b->info->time_nsec - node_a->info->time_nsec);
	}

	return compareName(a, b);
}

int compareSubdirName(void *a, void *b)
{
	t_subdir *node_a = a;
	t_subdir *node_b = b;
	return ft_strncmp(node_a->lower_name, node_b->lower_name, INT_MAX);
}

int compareSize(void *a, void *b)
{
	t_ls *node_a = a;
	t_ls *node_b = b;
	if (node_a->info->size_bytes == node_b->info->size_bytes)
		return compareName(a, b);
	return node_b->info->size_bytes - node_a->info->size_bytes;
}

int compareTimeArg(const void *a, const void *b)
{
	const char *path_a = *(const char **)a;
	const char *path_b = *(const char **)b;

	if (!path_a || !path_b) {
		return 0;
	}

	struct stat info_a;
	struct stat info_b;
	if (lstat(path_b, &info_b) == -1 || lstat(path_a, &info_a) == -1) {
		return 0;
	}

	if (info_a.st_mtime != info_b.st_mtime) {
		return info_b.st_mtime - info_a.st_mtime;
	}

	struct timespec time_spec_a = info_a.st_mtim;
	struct timespec time_spec_b = info_b.st_mtim;
	if (time_spec_a.tv_nsec != time_spec_b.tv_nsec) {
		return time_spec_b.tv_nsec - time_spec_a.tv_nsec;
	}

	return ft_strncmp(path_a, path_b, INT_MAX);
}

int compareSizeArg(const void *a, const void *b)
{
	char *path_a = *(void **)a;
	char *path_b = *(void **)b;

	if (!path_a || !path_b) {
		return 0;
	}

	struct stat info_a;
	struct stat info_b;
	if (lstat(path_a, &info_a) == -1 || lstat(path_b, &info_b) == -1) {
		return 0;
	}

	if (info_a.st_size != info_b.st_size) {
		return info_b.st_size - info_a.st_size;
	}
	return ft_strncmp(path_a, path_b, INT_MAX);
}