/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/04/22 17:37:27 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int explore_loop(t_data *data)
{
	int i = 0;
	
    ft_bubble_sort_string_arr(data->arg.all_path, ft_arr_len((void **)data->arg.all_path));
    if (data->arg.reverse){
        ft_arr_revert((void **)data->arg.all_path);
        
    }
    
    // for (i = 0; data->arg.all_path[i] != NULL; i++)
    // {
    //     printf("path: %s\n", data->arg.all_path[i]);
    // }
	while (data->arg.all_path[i] != NULL)
	{
        t_dll *list = malloc(sizeof(t_dll));
        dll_init(list);
        dll_insert_tail(list, data->list);
		if (exploreDirectories(data, data->list->tail->content, data->arg.all_path[i])){
            printf("exploreDirectories failed\n");
			return EXIT_FAILURE;
        }
		i++;
	}
    i = 0;
    t_node *node = data->list->head;
    while (node != NULL) {
        t_dll *dll = node->content;
        printf("%s:\n", data->arg.all_path[i]);
        output(data, dll);
        node = node->next;
        if (node == NULL)
            break;
        printf("\n");
        i++;
        // printf("ici \n");
    }
    // output(data, data->list);
	// dll_print_forward(list);
	return EXIT_SUCCESS;
}

// static int processEntry(t_dll *list, t_arg argList, char *path, 
//                        struct dirent *entry, t_format *format)
// {
//     char fullpath[PATH_MAX];
//     ft_memset(fullpath, 0, PATH_MAX);
//     appendStr(fullpath, path);
//     appendChar(fullpath, '/');
//     appendStr(fullpath, entry->d_name);
    
//     struct stat st;
//     if (lstat(fullpath, &st) != 0)
//         return EXIT_SUCCESS;  // Skip on stat failure
    
//     // Handle directories for recursive mode
//     if (S_ISDIR(st.st_mode) && 
//         ft_strncmp(".", entry->d_name, INT_MAX) != 0 &&
//         ft_strncmp("..", entry->d_name, INT_MAX) != 0 &&
//         argList.recurisve) {
        
//         if (!S_ISLNK(st.st_mode)) {
//             ft_printf(1, "\n%s:\n", fullpath);
//             return exploreDirectories(argList, fullpath);
//         }
//     }
    
//     // Skip hidden entries if not showing all
//     if (!argList.all && (ft_strncmp(".", entry->d_name, INT_MAX) == 0 ||
//         ft_strncmp("..", entry->d_name, INT_MAX) == 0))
//         return EXIT_SUCCESS;
    
//     t_ls_node *node = mallocLsNode();
//     if (!node)
//         return EXIT_FAILURE;
        
//     if (retrieveAllInfo(node, argList, path, entry, format)) {
//         // formatOutput(node, argList, format);
//         dll_insert_tail(node, list);
//         return EXIT_SUCCESS;
//     }
    
//     free(node);
//     return EXIT_SUCCESS;
// }
static void checkEntryType(t_ls *node, struct dirent *entry)
{
    node->name = ft_strdup(entry->d_name);
    if (entry->d_type == DT_DIR){
        node->type = DIRECTORY;
        node->is_dir = true;
    }
    else if (entry->d_type == DT_REG)
        node->type = FILE;
    else if (entry->d_type == DT_LNK){
        node->type = LINK;
        node->is_symbolic = true;
    }
    else
        node->type = UNKNOWN;
}

static int processEntry(t_data *data, t_ls *node, struct dirent *entry)
{
    if (ft_strncmp(".", entry->d_name, INT_MAX) == 0 ||
        ft_strncmp("..", entry->d_name, INT_MAX) == 0)
        return EXIT_SUCCESS;
    
    if (node->is_dir){
        printf("recursive directory\n");
        t_dll *sub = malloc(sizeof(t_dll));
        dll_init(sub);
        node->subdir = sub;
        // ft_printf(2, "\n%s:\n", "recursive directory");
        // ft_printf(2, "\n%s:\n", node->relative_path);
        exploreDirectories(data, node->subdir, node->relative_path);
    }
    if (retrieveAllInfo(data, node) == EXIT_FAILURE){
        printf("retrieveAllInfo failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int exploreDirectories(t_data *data, t_dll *list, char *path)
{   
    // Handle symlinks at the top level
    // struct stat path_stat;
    // if (lstat(path, &path_stat) == 0 && S_ISLNK(path_stat.st_mode))
    //     return handleSymlink(argList, path, &format);
    
    // Regular directory exploration
    printf("exploring %s\n", path);
    DIR *dir = opendir(path);
    if (!dir) {
        ft_printf(2, "ls: %s: %s\n", path, strerror(errno));
        return EXIT_FAILURE;
    }
    
    t_format *format = malloc(sizeof(t_format));
    initFormatStruct(format);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (ft_strncmp(".", entry->d_name, INT_MAX) == 0 ||
            ft_strncmp("..", entry->d_name, INT_MAX) == 0)
            continue;
        
        t_ls *node = mallocLs(format);
        if (!node) {
            closedir(dir);
            return EXIT_FAILURE;
        }
        
        node->relative_path = ft_join_path(path, entry->d_name);
        
        checkEntryType(node, entry);
        // printf("type: %d\n", node->type);
        // printf("entry: %s\n", entry->d_name);
        // printNodeLs(node);
       
        dll_insert_tail(node, list);
        // printf("inserted %s\n", node->name);
        if (processEntry(data, node, entry) == EXIT_FAILURE) {
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
    while (node != NULL) {
        // if (sizeof(node->content) == sizeof(t_ls)){
        //     printf("node t_ls\n");
        //     t_ls *ls = node->content;
        //     formatOutput(ls, data->arg);
        // }
        // else if (sizeof(node->content) == sizeof(t_dll)){
        //     printf("node t_dll\n");
        // }
        // else
        //     printf("node unknown\n");
        if (node->content == NULL){
            printf("node content is null\n");
            return EXIT_FAILURE;
        }
        // TODO: SEGV when there is a subdir
        t_ls *ls = node->content;
        // printNodeLs(ls);
        formatOutput(ls, data->arg);
        // printf("format: %s\n", ls->format);
        node = node->next;
    }
    
    // output(list, data->arg);
    return EXIT_SUCCESS;
}
