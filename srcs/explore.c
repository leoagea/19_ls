/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/04/01 19:28:46 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int explore_loop(t_arg argList)
{
	int i = 0;
	
	while (argList.all_path[i] != NULL)
	{
		if (!exploreDirectories(argList, argList.all_path[i]))
			return EXIT_FAILURE;
		i++;
	}
	// dll_print_forward(list);
	return EXIT_SUCCESS;
}

int exploreDirectories(t_arg argList, char *path)
{
    t_format format;
    initFormatStruct(&format);
    
    // First check if the input path itself is a symlink
    struct stat path_stat;
    if (lstat(path, &path_stat) == 0 && S_ISLNK(path_stat.st_mode)) {
        // Process the path as a symlink entry and return
        t_dll list;
        dll_init(&list);
        
        struct dirent entry;
        char *basename = strrchr(path, '/');
        if (basename == NULL)
            basename = path;
        else
            basename++; // Skip the '/'
            
        strcpy(entry.d_name, basename);
        entry.d_type = DT_LNK;
        
        t_ls_node *node = mallocLsNode();
        if (!node)
            return EXIT_FAILURE;
            
        char *parent_path = ft_strdup(path);
        char *last_slash = strrchr(parent_path, '/');
        if (last_slash)
            *last_slash = '\0';
        else
            strcpy(parent_path, ".");
            
        if (retrieveAllInfo(node, argList, parent_path, &entry, &format)) {
            formatOutput(node, argList, &format);
            dll_insert_tail(node, &list);
        }
        free(parent_path);
        output(&list, argList);
        return EXIT_SUCCESS;
    }
    
    // Normal directory exploration
    t_dll list;
    dll_init(&list);
    
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
        char fullpath[PATH_MAX];
        ft_memset(fullpath, 0, PATH_MAX);
        appendStr(fullpath, path);
        appendChar(fullpath, '/');
        appendStr(fullpath, entry->d_name);
        
        struct stat st;
        if (lstat(fullpath, &st) != 0) {
            // Handle stat failure
            continue;
        }
        
        // Check if it's a directory we need to recurse into
        if (S_ISDIR(st.st_mode) 
            && ft_strncmp(".", entry->d_name, INT_MAX) != 0
            && ft_strncmp("..", entry->d_name, INT_MAX) != 0
            && argList.recurisve) {
            
            // Don't recurse if it's a symlink to a directory
            if (S_ISLNK(st.st_mode)) {
                // Handle as a regular entry
                t_ls_node *node = mallocLsNode();
                if (!node)
                    return EXIT_FAILURE;
                    
                if (retrieveAllInfo(node, argList, path, entry, &format)) {
                    formatOutput(node, argList, &format);
                    dll_insert_tail(node, &list);
                } else {
                    free(node);
                    return EXIT_FAILURE;
                }
            } else {
                // For real directories, recurse but don't return immediately
                ft_printf(1, "\n%s:\n", fullpath);
                exploreDirectories(argList, fullpath);
            }
        } else {
            // Handle non-directory entries or those we don't recurse into
            if (!argList.all && (ft_strncmp(".", entry->d_name, INT_MAX) == 0
                || ft_strncmp("..", entry->d_name, INT_MAX) == 0))
                continue;
    
            t_ls_node *node = mallocLsNode();
            if (!node)
                return EXIT_FAILURE;
                
            if (retrieveAllInfo(node, argList, path, entry, &format)) {
                formatOutput(node, argList, &format);
                dll_insert_tail(node, &list);
            } else {
                free(node);
                return EXIT_FAILURE;
            }
        }
    }
    
    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }
    
    // printFormatStruct(&format);
    output(&list, argList);
    return EXIT_SUCCESS;
}

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
	node->name = entry->d_name;
	node->type = entry->d_type;
	node->relative_path = relPath;
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