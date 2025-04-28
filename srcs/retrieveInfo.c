/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieveInfo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:18:08 by lagea             #+#    #+#             */
/*   Updated: 2025/04/28 19:00:19 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int retrieveAllInfo(t_data *data, t_ls *node)
{
	t_info *info_tmp = malloc(sizeof(t_info));
	initInfoStruct(info_tmp);
	
	struct stat info;
	if (node->is_symbolic ? lstat(node->relative_path, &info) : stat(node->relative_path, &info) == -1)
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
    
	node->info = info_tmp;
	
    if (node->is_symbolic)
		handleSymlink(node);
	
    getFormatLen(node, node->format_info);

	return EXIT_SUCCESS;
}
