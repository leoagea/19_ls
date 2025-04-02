/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/04/02 16:59:56 by lagea            ###   ########.fr       */
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

static int handleSymlink(t_arg argList, char *path, t_format *format)
{
    t_dll list;
    dll_init(&list);
    
    struct dirent entry;
    char *basename = strrchr(path, '/');
    basename = (basename == NULL) ? path : basename + 1;
    ft_strlcpy(entry.d_name, basename, NAME_MAX);
    entry.d_type = DT_LNK;
    
    t_ls_node *node = mallocLsNode();
    if (!node)
        return EXIT_FAILURE;
        
    char *parent_path = ft_strdup(path);
    char *last_slash = ft_strrchr(parent_path, '/');
    if (last_slash)
        *last_slash = '\0';
    else
        ft_strlcpy(parent_path, ".", PATH_MAX);
        
    int success = retrieveAllInfo(node, argList, parent_path, &entry, format);
    if (success) {
        formatOutput(node, argList, format);
        dll_insert_tail(node, &list);
    }
    free(parent_path);
    output(&list, argList);
    return EXIT_SUCCESS;
}

static int processEntry(t_dll *list, t_arg argList, char *path, 
                       struct dirent *entry, t_format *format)
{
    char fullpath[PATH_MAX];
    ft_memset(fullpath, 0, PATH_MAX);
    appendStr(fullpath, path);
    appendChar(fullpath, '/');
    appendStr(fullpath, entry->d_name);
    
    struct stat st;
    if (lstat(fullpath, &st) != 0)
        return EXIT_SUCCESS;  // Skip on stat failure
    
    // Handle directories for recursive mode
    if (S_ISDIR(st.st_mode) && 
        ft_strncmp(".", entry->d_name, INT_MAX) != 0 &&
        ft_strncmp("..", entry->d_name, INT_MAX) != 0 &&
        argList.recurisve) {
        
        if (!S_ISLNK(st.st_mode)) {
            ft_printf(1, "\n%s:\n", fullpath);
            return exploreDirectories(argList, fullpath);
        }
    }
    
    // Skip hidden entries if not showing all
    if (!argList.all && (ft_strncmp(".", entry->d_name, INT_MAX) == 0 ||
        ft_strncmp("..", entry->d_name, INT_MAX) == 0))
        return EXIT_SUCCESS;
    
    t_ls_node *node = mallocLsNode();
    if (!node)
        return EXIT_FAILURE;
        
    if (retrieveAllInfo(node, argList, path, entry, format)) {
        formatOutput(node, argList, format);
        dll_insert_tail(node, list);
        return EXIT_SUCCESS;
    }
    
    free(node);
    return EXIT_FAILURE;
}

int exploreDirectories(t_arg argList, char *path)
{
    t_format format;
    initFormatStruct(&format);
    
    // Handle symlinks at the top level
    struct stat path_stat;
    if (lstat(path, &path_stat) == 0 && S_ISLNK(path_stat.st_mode))
        return handleSymlink(argList, path, &format);
    
    // Regular directory exploration
    t_dll list;
    dll_init(&list);
    
    DIR *dir = opendir(path);
    if (!dir) {
        ft_printf(2, "ls: %s: %s\n", path, strerror(errno));
        return EXIT_FAILURE;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (processEntry(&list, argList, path, entry, &format) == EXIT_FAILURE) {
            closedir(dir);
            return EXIT_FAILURE;
        }
    }
    
    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }
    
    output(&list, argList);
    return EXIT_SUCCESS;
}
