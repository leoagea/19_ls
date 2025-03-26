/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/03/26 19:15:50 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int explore_loop(t_dll *list, t_arg argList)
{
	int i = 0;
	
	while (argList.all_path[i] != NULL)
	{
		if (exploreDirectories(argList, list, argList.all_path[i]))
			return EXIT_FAILURE;
		i++;
	}
	// dll_print_forward(list);
	return EXIT_SUCCESS;
}

int exploreDirectories(t_arg argList, t_dll *list, char *path)
{
	DIR *dir = opendir(path);
    if (dir == NULL)
    {
		char *err_msg = strerror(errno);
		ft_printf(2, "ls: %s: %s\n", path, err_msg);
        return EXIT_FAILURE;
    }
	struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
		// if (entry->d_type == UNKNOWN)
		// 	continue;
		if (entry->d_type == DIRECTORY 
			&& ft_strncmp(".", entry->d_name, INT_MAX) != 0
			&& ft_strncmp("..", entry->d_name, INT_MAX) != 0){
				
			char *suffix = ft_strjoin("/", entry->d_name);
			char *newpath = ft_strjoin(path, suffix);
			free(suffix);
			exploreDirectories(argList, list, newpath);
		}
		else{
			if (!argList.all && (ft_strncmp(".", entry->d_name, INT_MAX) == 0
				|| ft_strncmp("..", entry->d_name, INT_MAX) == 0))
					continue;
	
			t_ls_node *new = newLsNode(argList, path, entry);
			if (!new)
				return EXIT_FAILURE;
			dll_insert_tail(new, list);
		}
    }
	if (closedir(dir) == -1)
    {
        perror("closedir");
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}


t_ls_node *newLsNode(t_arg arg, char *path, struct dirent *entry)
{
	t_ls_node *node = malloc(sizeof(t_ls_node));
	if (!node)
		return NULL;
	initLsNode(node);
	
	char *tmp = ft_strjoin(path, "/");
	char *relPath = ft_strjoin(tmp, entry->d_name);
	free(tmp);

	struct stat info;
	if (stat(relPath, &info) == -1)
		return (printf("stat failed\n"), free(relPath), NULL);

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
	node->name = entry->d_name;
	node->type = entry->d_type;
	node->relative_path = relPath;
	node->last_mod = extractTimeModified(info);

	if (entry->d_type == LINK){
		size_t size = sizeof(node->sym_name);
		
		node->symbolic = true;
		// if (lstat(entry->d_name, &info) == -1)
		// 	return (printf("lstat failed\n"), NULL);
		ft_memset(node->sym_name, 0, size);
		if (arg.long_format){
			if (readlink(node->relative_path, node->sym_name, size) == -1)
				return (printf("readlink failed\n"), NULL);
			node->size_bytes = ft_strlen(node->sym_name);
		}
	}
	else
		node->symbolic = false;
	
	return node;
}