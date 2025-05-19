/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/05/19 15:39:43 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int explore_loop(t_data *data)
{
	int i = 0;

	ft_bubble_sort_string_arr(data->arg.all_path,
							  ft_arr_len((void **)data->arg.all_path));
	if (data->arg.reverse) {
		ft_arr_revert((void **)data->arg.all_path);
	}

	while (data->arg.all_path[i] != NULL) {
		t_dll *list = malloc(sizeof(t_dll));
		dll_init(list);
		dll_insert_tail(list, data->list);
		if (exploreDirectories(data, data->list->tail->content,
							   data->arg.all_path[i])) {
			printf("exploreDirectories failed\n");
			return EXIT_FAILURE;
		}
		i++;
	}

	int tmp = i;
	i = 0;
	t_node *node = data->list->head;
	while (node != NULL) {
		t_dll *dll = node->content;
		if (tmp != 1)
			printf("%s:\n", data->arg.all_path[i]);
		output(data, dll);
		node = node->next;
		if (node == NULL)
			break;
		printf("\n");
		i++;
	}
	return EXIT_SUCCESS;
}

static void checkEntryType(t_ls *node, struct dirent *entry)
{
	if (entry->d_type == DT_DIR) {
		node->type = DIRECTORY;
		node->is_dir = true;
	} else if (entry->d_type == DT_REG)
		node->type = REGFILE;
	else if (entry->d_type == DT_LNK) {
		node->type = LINK;
		node->is_symbolic = true;
	} else if (entry->d_type == DT_CHR)
		node->type = CHARFILE;
	else if (entry->d_type == DT_WHT)
		node->type = CFS;
	else if (entry->d_type == DT_BLK)
		node->type = BLKFILE;
	else if (entry->d_type == DT_FIFO)
		node->type = FIFO;
	else if (entry->d_type == DT_SOCK)
		node->type = SOCKET;
	else
		node->type = UNKNOWN;
}

int handleSymlink(t_ls *node)
{
	char *link_path = node->relative_path[0] == '/'
						  ? ft_strdup(node->relative_path)
						  : ft_join_path(".", node->relative_path);

	if (!link_path) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	if (!node->info) {
		perror("malloc");
		free(link_path);
		return EXIT_FAILURE;
	}

	ssize_t len = readlink(link_path, node->info->sym_name,
						   sizeof(node->info->sym_name) - 1);
	if (len == -1) {
		perror("readlink");
		return EXIT_FAILURE;
	}

	node->info->size_bytes = ft_strlen(node->info->sym_name);
	node->info->size_bytes_len = ft_intlen(node->info->size_bytes);

	return EXIT_SUCCESS;
}

static int processEntry(t_data *data, t_ls *node)
{
	// struct stat path_stat;
	if (node->is_symbolic)
		return retrieveAllInfo(data, node);

	if (node->is_dir && data->arg.recurisve &&
		(ft_strncmp(".", node->name, INT_MAX) != 0 &&
		 ft_strncmp("..", node->name, INT_MAX) != 0)) {
		t_dll *sub = malloc(sizeof(t_dll));
		dll_init(sub);
		node->subdir = sub;
		exploreDirectories(data, node->subdir, node->relative_path);
	}

	return retrieveAllInfo(data, node);
}

int exploreDirectories(t_data *data, t_dll *list, char *path)
{
	DIR *dir = opendir(path);
	if (!dir) {
		ft_printf(2, "ls: %s: %s\n", path, strerror(errno));
		return EXIT_FAILURE;
	}

	t_format *format = malloc(sizeof(t_format));
	initFormatStruct(format);
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		// if ((ft_strncmp(".", entry->d_name, INT_MAX) == 0 ||
		//     ft_strncmp("..", entry->d_name, INT_MAX) == 0) &&
		//     !data->arg.all)
		//     continue;
		if (entry->d_name[0] == '.' && !data->arg.all)
			continue;

		t_ls *node = mallocLs(format);
		if (!node) {
			closedir(dir);
			return EXIT_FAILURE;
		}

		node->name = ft_strdup(entry->d_name);
		node->relative_path = ft_join_path(path, entry->d_name);

		checkEntryType(node, entry);

		dll_insert_tail(node, list);

		if (processEntry(data, node) == EXIT_FAILURE) {
			printf("processEntry failed\n");
			closedir(dir);
			return EXIT_FAILURE;
		}
	}

	if (closedir(dir) == -1) {
		perror("closedir");
		return EXIT_FAILURE;
	}

	t_node *node = list->head;
	// t_ls *tmp = node->content;
	// printFormatStruct(tmp->format_info); // For debugging
	while (node != NULL) {
		if (node->content == NULL) {
			printf("node content is null\n");
			return EXIT_FAILURE;
		}

		t_ls *ls = node->content;
		// printNodeLs(ls);
		formatOutput(ls, data->arg);
		node = node->next;
	}

	return EXIT_SUCCESS;
}
