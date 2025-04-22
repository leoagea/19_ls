/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieveInfo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:18:08 by lagea             #+#    #+#             */
/*   Updated: 2025/04/22 18:09:41 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int retrieveAllInfo(t_data *data, t_ls *node)
{
	t_info *info_tmp = malloc(sizeof(t_info));
	initInfoStruct(info_tmp);
	
	struct stat info;
	if (stat(node->relative_path, &info) == -1)
		return (printf("stat failed\n"), EXIT_FAILURE);

	if (data->arg.long_format){
		if (node->type == DIRECTORY)
			info_tmp->size_bytes = 4096; //linux, how macos handle ? not the same way as linux for sure
		info_tmp->size_bytes = info.st_size;
		info_tmp->nlink = info.st_nlink;
		
		struct passwd *pwd = getpwuid(info.st_uid);
		struct group *grp = getgrgid(info.st_gid);
		if (pwd && grp){
			info_tmp->user_name = ft_strdup(pwd->pw_name);
			info_tmp->group_name = ft_strdup(grp->gr_name);
	
		}
		else{
			info_tmp->user_name = ft_strdup("unknown");
			info_tmp->group_name = ft_strdup("unknown");
		}
		extractPerm(info_tmp->perm, info.st_mode);
		
		char *tmp_nlink = ft_itoa(info_tmp->nlink);
		char *tmp_size = ft_itoa(info_tmp->size_bytes);
		info_tmp->nlink_len = ft_strlen(tmp_nlink);
		info_tmp->user_name_len = ft_strlen(info_tmp->user_name);
		info_tmp->group_name_len = ft_strlen(info_tmp->group_name);
		info_tmp->size_bytes_len = ft_strlen(tmp_size);
		free(tmp_nlink);
		free(tmp_size);
		info_tmp->last_mod = extractTimeModified(info);
	}
	

	// if (node->d_type == LINK){
	// 	retrieveSymInfo(node, arg);
	// }
	// else
	// 	node->symbolic = false;
    
    
    
	node->info = info_tmp;
    getFormatLen(node, node->format_info);

	return EXIT_SUCCESS;
}

// int retrieveSymInfo(t_ls_node *node, t_arg arg)
// {
//     size_t size = sizeof(node->sym_name);
	
// 	node->symbolic = true;
// 	// if (lstat(entry->d_name, &info) == -1)
// 	// 	return (printf("lstat failed\n"), NULL);
// 	if (arg.long_format){
// 		if (readlink(node->relative_path, node->sym_name, size) == -1)
// 			return (printf("readlink failed\n"), 0);
// 		node->size_bytes = ft_strlen(node->sym_name);
// 	}
// 	return 1;
// }