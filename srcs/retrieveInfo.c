/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieveInfo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:18:08 by lagea             #+#    #+#             */
/*   Updated: 2025/05/29 15:59:06 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void extractTime(t_arg arg, struct stat info, char *str)
{
	time_t now = time(NULL);
	time_t mod_sec = (arg.access_time) ? info.st_atime : info.st_mtime;
	time_t six_months = 15552000;
	char  *full_time_str = ctime(&mod_sec);

	if (!full_time_str) {
		perror("ctime");
		return;
	}

	char *time_str = str;
	ft_memset(time_str, 0, 13);

	if ((now - mod_sec > six_months) || (mod_sec > now)) {
		ft_memcpy(time_str, &full_time_str[4], 7);
		ft_memcpy(time_str + 7, &full_time_str[19], 5);
	} else {
		ft_memcpy(time_str, &full_time_str[4], 12);
		time_str[12] = '\0';
	}
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

t_xattr *create_xattr_node(const char *path, const char *attr_name)
{
	t_xattr *node;
	ssize_t	 value_size;

	node = malloc(sizeof(t_xattr));
	if (!node)
		return (NULL);

	node->name = ft_strdup(attr_name);
	node->next = NULL;

	if (!node->name) {
		free(node);
		return (NULL);
	}

	// Get size of attribute value
	value_size = GET_XATTR(path, attr_name, NULL, 0);
	if (value_size == -1) {
		free(node->name);
		free(node);
		return (NULL);
	}

	node->value_size = value_size;
	node->value = malloc(value_size);
	if (!node->value) {
		free(node->name);
		free(node);
		return (NULL);
	}

	// Get actual attribute value
	if (GET_XATTR(path, attr_name, node->value, value_size) == -1) {
		free(node->name);
		free(node->value);
		free(node);
		return (NULL);
	}

	return (node);
}

void get_file_xattr(t_ls *node, const char *path)
{
	ssize_t list_size;
	char   *attr_list;
	char   *curr_attr;

	// Get size needed for attributes list
	list_size = LIST_XATTR(path, NULL, 0);
	if (list_size <= 0)
		return;

	attr_list = malloc(list_size);
	if (!attr_list)
		return;

	// Get actual list
	if (LIST_XATTR(path, attr_list, list_size) == -1) {
		free(attr_list);
		return;
	}

	curr_attr = attr_list;
	while (curr_attr < attr_list + list_size) {
		t_xattr *new = create_xattr_node(path, curr_attr);
		if (new)
			dll_insert_tail(new, node->xattr_list);
		curr_attr += strlen(curr_attr) + 1;
	}

	free(attr_list);
}

static void retrieveMajorMinor(t_info **ls, struct stat info)
{
	if (!ls || !*ls)
		return;

	int	  major;
	int	  minor;
	char *format = (*ls)->major;

	major = major(info.st_rdev);
	minor = minor(info.st_rdev);

	if (major > 4095) {
		major = 4095;
	}
	if (minor > 1048575) {
		minor = 1048575;
	}

	char *major_str = ft_itoa(major);
	char *minor_str = ft_itoa(minor);
	if (!major_str || !minor_str) {
		freeStr(&major_str);
		freeStr(&minor_str);
		return;
	}

	ft_memcpy(format, major_str, ft_strlen(major_str));
	ft_memcpy(format + ft_strlen(major_str), ", ", 2);
	ft_memcpy(format + ft_strlen(major_str) + 2, minor_str, ft_strlen(minor_str));

	freeStr(&major_str);
	freeStr(&minor_str);
}

static int extractLongFormat(t_data *data, t_ls *node, struct stat info, t_info *info_tmp)
{
	info_tmp->nlink = info.st_nlink;

	struct passwd *pwd = getpwuid(info.st_uid);
	struct group  *grp = getgrgid(info.st_gid);
	if (pwd && grp) {
		info_tmp->user_id = pwd->pw_uid;
		info_tmp->user_id_len = ft_intlen(info_tmp->user_id);
		info_tmp->user_name = ft_strdup(pwd->pw_name);

		info_tmp->group_id = grp->gr_gid;
		info_tmp->group_id_len = ft_intlen(info_tmp->group_id);
		info_tmp->group_name = ft_strdup(grp->gr_name);
	} else {
		info_tmp->user_name = ft_strdup("unknown");
		info_tmp->group_name = ft_strdup("unknown");
	}

	info_tmp->nlink_len = ft_intlen(info_tmp->nlink);

	info_tmp->user_name_len = ft_strlen(info_tmp->user_name);
	info_tmp->group_name_len = ft_strlen(info_tmp->group_name);

	if (node->type == BLKFILE || node->type == CHARFILE) {
		retrieveMajorMinor(&info_tmp, info);
		info_tmp->size_bytes_len = ft_strlen(info_tmp->major);
	}

	if (LIST_XATTR(node->relative_path, NULL, 0) > 0) {
		info_tmp->perm[9] = '@';

		if (data->arg.extended_attributes) {
			node->xattr_list = malloc(sizeof(t_dll));
			if (!node->xattr_list)
				return (EXIT_FAILURE);
			dll_init(node->xattr_list);
			get_file_xattr(node, node->relative_path);
		}
	}
	return EXIT_SUCCESS;
}

static int extractSizeInfo(t_data *data, t_ls *node, struct stat info, t_info *info_tmp)
{
	info_tmp->block_size = CALC_BLOCKS((int)info.st_blocks);

	info_tmp->block_size_len = ft_intlen(info_tmp->block_size);

	info_tmp->size_bytes = info.st_size;
	info_tmp->size_bytes_len = ft_intlen(info_tmp->size_bytes);
	if (data->arg.comma) {
		info_tmp->size_thousands = (info_tmp->size_bytes_len - 1) / 3;
		info_tmp->size_bytes_len += info_tmp->size_thousands;
	} else if (data->arg.human_readable) {
		if ((node->type == REGFILE || node->type == DIRECTORY) && info_tmp->size_bytes > 0) {
			char *human_readable = get_human_readable_size(info_tmp->size_bytes);
			if (!human_readable) {
				info_tmp->size_bytes_len = 1;
				return (EXIT_FAILURE);
			}
			info_tmp->size_bytes_len = ft_strlen(human_readable);
			free(human_readable);
		}
	}
	return EXIT_SUCCESS;
}

int retrieveAllInfo(t_data *data, t_ls *node, t_format **format)
{
	t_info *info_tmp = malloc(sizeof(t_info));
	initInfoStruct(info_tmp);

	struct stat info;
	if (node->is_symbolic ? lstat(node->relative_path, &info)
						  : stat(node->relative_path, &info) == -1) {
		free(info_tmp);
		return (ft_printf(2, "ls: cannot access '%s': %s\n", node->relative_path, strerror(errno)),
				EXIT_FAILURE);
	}

	extractSizeInfo(data, node, info, info_tmp);

	if (node->type == DIRECTORY && data->arg.slash) {
		char *tmp = ft_strjoin(node->name, "/");
		free(node->name);
		node->name = tmp;
	}

	info_tmp->time_info = (data->arg.access_time) ? info.st_atime : info.st_mtime;
	struct timespec time_spec = (data->arg.access_time) ? info.st_atim : info.st_mtim;
	info_tmp->time_nsec = time_spec.tv_nsec; // For nanosecond precision
	extractTime(data->arg, info, info_tmp->time);

	extractPerm(info_tmp->perm, info.st_mode);

	if (data->arg.long_format) {
		extractLongFormat(data, node, info, info_tmp);	
	}

	info_tmp->name_len = ft_strlen(node->name) + 1;

	node->info = info_tmp;

	if (node->is_symbolic)
		handleSymlink(node);

	getFormatLen(node, format);

	return EXIT_SUCCESS;
}
