/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/06/05 14:17:11 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static int directory(t_data *data)
{	
	t_format *format = malloc(sizeof(t_format));
	initFormatStruct(format);
	
	t_input *current = data->arg.input_list;
	while (current != NULL) {
		t_ls *node = mallocLs();
		if (!node) {
			return EXIT_FAILURE;
		}
		node->name = ft_strdup(current->name);
		node->relative_path = ft_strdup(current->name);
		node->is_dir = true;
		node->type = DIRECTORY;
		dll_insert_tail(node, data->list);
		if (processEntry(data, node, &format) == EXIT_FAILURE) {
			freeLsNode(node);
			dll_free(data->list, freeLsNode);
			return EXIT_FAILURE;
		}
		current = current->next;
	}

	t_node *node = data->list->head;
	while (node != NULL) {
		if (node->content == NULL) {
			return EXIT_FAILURE;
		}

		t_ls *ls = node->content;
		formatOutput(format, ls, data->arg);
		node = node->next;
	}

	if (format) {
		freeFormatStruct(&format);
	}

	output(data, data->list);
	dll_free(data->list, freeLsNode);
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

static void checkFileType(t_ls *node, struct stat *path_stat)
{
	if (S_ISDIR(path_stat->st_mode)) {
		node->type = DIRECTORY;
		node->is_dir = true;
	} else if (S_ISREG(path_stat->st_mode)) {
		node->type = REGFILE;
	} else if (S_ISLNK(path_stat->st_mode)) {
		node->type = LINK;
		node->is_symbolic = true;
	} else if (S_ISCHR(path_stat->st_mode)) {
		node->type = CHARFILE;
	} else if (S_ISBLK(path_stat->st_mode)) {
		node->type = BLKFILE;
	} else if (S_ISFIFO(path_stat->st_mode)) {
		node->type = FIFO;
	} else if (S_ISSOCK(path_stat->st_mode)) {
		node->type = SOCKET;
	} else {
		node->type = UNKNOWN;
	}
}

int handleSymlink(t_ls *node)
{
	char *link_path = node->relative_path[0] == '/' ? ft_strdup(node->relative_path)
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

	ssize_t len = readlink(link_path, node->info->sym_name, sizeof(node->info->sym_name) - 1);
	if (len == -1) {
		free(link_path);
		ft_printf(2, "ls: cannot read symbolic link '%s': %s\n", node->relative_path,
				  strerror(errno));
		return EXIT_FAILURE;
	}
	free(link_path);
	node->info->size_bytes = ft_strlen(node->info->sym_name);
	node->info->size_bytes_len = ft_intlen(node->info->size_bytes);

	return EXIT_SUCCESS;
}

int processEntry(t_data *data, t_ls *node, t_format **format)
{
	if (node->is_symbolic)
		return retrieveAllInfo(data, node, format);

	if (node->is_dir && data->arg.recurisve &&
		(ft_strncmp(".", node->name, INT_MAX) != 0 && ft_strncmp("..", node->name, INT_MAX) != 0)) {
		t_dll *sub = malloc(sizeof(t_dll));
		if (!sub) {
			freeLsNode(node);
			return EXIT_FAILURE;
		}
		dll_init(sub);
		node->subdir = sub;
		if (exploreDirectories(data, node->subdir, node->relative_path) == EXIT_FAILURE) {
			dll_free(sub, freeLsNode);
			data->return_val = 1;
			return EXIT_FAILURE;
		}
	}

	if (retrieveAllInfo(data, node, format) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int exploreDirectories(t_data *data, t_dll *list, char *path)
{
	DIR *dir = opendir(path);
	if (!dir) {
		ft_printf(2, "ls: cannot open directory '%s': %s\n", path, strerror(errno));
		return (data->return_val = 2, EXIT_SUCCESS);
	}

	t_format *format = malloc(sizeof(t_format));
	if (!format) {
		closedir(dir);
		perror("malloc");
		return EXIT_FAILURE;
	}
	initFormatStruct(format);
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !data->arg.all)
			continue;

		t_ls *node = mallocLs();
		if (!node) {
			perror("malloc");
			continue;
		}

		node->name = ft_strdup(entry->d_name);
		node->lower_name = string_to_lower(node->name);
		node->relative_path = path[ft_strlen(path) - 1] == '/' ? ft_strjoin(path, entry->d_name)
															   : ft_join_path(path, entry->d_name);

		if (!node->name || !node->lower_name || !node->relative_path) {
			freeLsNode(node);
			perror("malloc");
			continue;
		}
		
		checkEntryType(node, entry);

		dll_insert_tail(node, list);

		processEntry(data, node, &format);
	}

	if (closedir(dir) == -1) {
		freeFormatStruct(&format);
		perror("closedir");
		return EXIT_FAILURE;
	}

	t_node *node = list->head;
	while (node != NULL) {
		if (node->content == NULL) {
			return EXIT_FAILURE;
		}

		t_ls *ls = node->content;
		formatOutput(format, ls, data->arg);
		node = node->next;
	}
	if (format) {
		freeFormatStruct(&format);
	}

	return EXIT_SUCCESS;
}

static void output_loop(t_data *data)
{
	static t_input *list;
	list = data->arg.input_list;
	
	t_node *node = data->list->head;
	while (node != NULL) {
		t_dll *dll = node->content;
		if (!dll || dll->head == NULL) {
			node = node->next;
			ft_printf(1, "%s %d\n", TOTAL_BLOCKS, 0);
			continue;
		}
		if (data->arg.all_paths_len > 1 || data->arg.recurisve){
			while (list && list->type != DIRECTORY && list->next)
				list = list->next;
			ft_printf(1, "%s:\n",list->name);
			list = list->next;
		}
		output(data, dll);
		node = node->next;
		if (node == NULL)
			break;
		ft_printf(1, "\n");
	}
}

static int exploreFile(t_data *data, t_dll *list, char *path, t_format **format)
{
	t_ls *node = mallocLs();
	if (!node) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	node->name = ft_strdup(path);
	node->lower_name = string_to_lower(node->name);
	node->relative_path = ft_strdup(path);
	if (!node->name || !node->relative_path || !node->lower_name) {
		freeLsNode(node);
		perror("malloc");
		return EXIT_FAILURE;
	}

	node->is_dir = false;

	struct stat path_stat;
	if (lstat(path, &path_stat) == -1) {
		return EXIT_FAILURE;
	}
	
	checkFileType(node, &path_stat);

	dll_insert_tail(node, list);
	
	if (processEntry(data, node, format) == EXIT_FAILURE) {
		freeLsNode(node);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;	
}

int explore_loop(t_data *data)
{
	if (data->arg.directory)
		return directory(data);

	t_format *format = malloc(sizeof(t_format));
	if (!format) {
		perror("malloc");
		return EXIT_FAILURE;
	}
	
	initFormatStruct(format);
	t_input *input = data->arg.input_list;
	// debug_print_input_list(data->arg.input_list);
	while (input != NULL) {
		if (input->type == DIRECTORY){
			t_dll *list = malloc(sizeof(t_dll));
			dll_init(list);
			dll_insert_tail(list, data->list);
			
			if (exploreDirectories(data, data->list->tail->content, input->name) ==
				EXIT_FAILURE) {
				freeFormatStruct(&format);
				return EXIT_FAILURE;
			}
		}
		else if (input->type == REGFILE) {
			if (exploreFile(data, data->file_list, input->name, &format) == EXIT_FAILURE) {
				freeFormatStruct(&format);
				return EXIT_FAILURE;
			}
		}
		
		input = input->next;
	}
	
	t_node *node = data->file_list->head;
	while (node != NULL) {
		if (node->content == NULL) {
			return EXIT_FAILURE;
		}

		t_ls *ls = node->content;
		formatOutput(format, ls, data->arg);
		node = node->next;
	}
	if (format) {
		freeFormatStruct(&format);
	}
	
	if (data->file_list->size != 0)
		outputListFiles(data, data->file_list);
	if (data->list->size != 0)
		output_loop(data);

	return EXIT_SUCCESS;
}

