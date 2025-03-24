/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:52:02 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 23:42:36 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int explore_loop(t_dll *list, t_arg arglist)
{
	int i = 0;
	
	while (arglist.all_path[i] != NULL)
	{
		if (exploreDirectories(list, arglist.all_path[i]))
			return EXIT_FAILURE;
		i++;
	}
	dll_print_forward(list);
	return EXIT_SUCCESS;
}

int exploreDirectories(t_dll *list, char *path)
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
		if (entry->d_type == DIRECTORY && ft_strncmp(".", entry->d_name, INT_MAX) != 0
			&& ft_strncmp("..", entry->d_name, INT_MAX) != 0){
				
			char *suffix = ft_strjoin("/", entry->d_name);
			char *newpath = ft_strjoin(path, suffix);
			free(suffix);
			exploreDirectories(list, newpath);
		}
		else{
			dll_insert_tail(entry, list);
		}
    }
	if (closedir(dir) == -1)
    {
        perror("closedir");
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}