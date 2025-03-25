/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/03/25 19:18:44 by lagea            ###   ########.fr       */
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
	dll_print_forward(list);
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
	
			t_ls_node *new = newLsNode(path, entry);
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