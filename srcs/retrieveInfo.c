/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieveInfo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:18:08 by lagea             #+#    #+#             */
/*   Updated: 2025/04/03 16:22:06 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int retrieveAllInfo(t_ls_node *node, t_arg arg, char *path, struct dirent *entry, t_format *format)
{
	char *tmp = ft_strjoin(path, "/");
	char *relPath = ft_strjoin(tmp, entry->d_name);
	free(tmp);

	struct stat info;
	if (stat(relPath, &info) == -1)
		return (printf("stat failed\n"), free(relPath), 0);

	if (arg.long_format){
		if (entry->d_type == DIRECTORY)
			node->size_bytes = 4096; //linux, how macos handle ? not the same way as linux for sure
		node->size_bytes = info.st_size;
		node->nlink = info.st_nlink;
		
		struct passwd *pwd = getpwuid(info.st_uid);
		struct group *grp = getgrgid(info.st_gid);
		if (pwd && grp){
			node->user_name = ft_strdup(pwd->pw_name);
			node->group_name = ft_strdup(grp->gr_name);
	
		}
		else{
			node->user_name = ft_strdup("unknown");
			node->group_name = ft_strdup("unknown");
		}
		extractPerm(node->perm, info.st_mode);
	}
	
	node->info = &info;
	node->entry = entry;
	node->name = ft_strdup(entry->d_name);
	node->type = entry->d_type;
	node->relative_path = ft_strdup(relPath);
	free(relPath);
	node->last_mod = extractTimeModified(info);

	if (entry->d_type == LINK){
		retrieveSymInfo(node, arg);
	}
	else
		node->symbolic = false;
    
    char *tmp_nlink = ft_itoa(node->nlink);
    char *tmp_size = ft_itoa(node->size_bytes);
    node->nlink_len = ft_strlen(tmp_nlink);
    node->user_name_len = ft_strlen(node->user_name);
    node->group_name_len = ft_strlen(node->group_name);
    node->size_bytes_len = ft_strlen(tmp_size);
    free(tmp_nlink);
    free(tmp_size);
    
    getFormatLen(node, format);
	
	return 1;
}

int retrieveSymInfo(t_ls_node *node, t_arg arg)
{
    size_t size = sizeof(node->sym_name);
	
	node->symbolic = true;
	// if (lstat(entry->d_name, &info) == -1)
	// 	return (printf("lstat failed\n"), NULL);
	if (arg.long_format){
		if (readlink(node->relative_path, node->sym_name, size) == -1)
			return (printf("readlink failed\n"), 0);
		node->size_bytes = ft_strlen(node->sym_name);
	}
	return 1;
}