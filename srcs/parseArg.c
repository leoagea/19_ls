/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseArg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:30:57 by lagea             #+#    #+#             */
/*   Updated: 2025/05/13 00:55:05 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static int getAllPath(int ac, char **av, t_arg *argList, int i)
{
	while (i < ac){
		char *tmp = NULL;
		if (!argList->path)
			tmp = ft_strdup(av[i]);
		else{
			tmp = ft_strjoin(argList->path, av[i]);
			free(argList->path);
		}
		argList->path = ft_strjoin(tmp, " ");
		if (tmp)
			free(tmp);
		i++;
	}
	
	if (!argList->path){
		argList->all_path = malloc(2 * sizeof(char*));
		if (!argList->all_path)
			return EXIT_FAILURE;
		argList->all_path[0] = ft_strdup(DEFAULT_PATH);
		argList->all_path[1] = NULL;
	}
	else{
		argList->all_path = ft_split(argList->path, ' ');
	}

	return EXIT_SUCCESS;
}

int parseArg(int ac, char **av, t_arg *argList)
{
	int i = 0;
	
	//Parse all options
	while (++i < ac && av[i][0] == '-'){
		int j = 0;
		
		if (av[i][1] == '-'){
			if (ft_strncmp(av[i], "--help", INT_MAX) == 0){
				help();
				return EXIT_CLEAN;
			}
			else if (ft_strncmp(av[i], "--version", INT_MAX) == 0){
				ft_printf(1, "ft_ls version 1.0\n");
				return EXIT_CLEAN;
			}
		}
		
		while (av[i][++j]){
			char option = av[i][j];
			
			switch (option){
				case 'a':
					argList->all = true;
					break;
				
				case 'r':
					argList->reverse = true;
					break;
					
				case 't':
					argList->sort_time = true;
					break;
					
				case 'R':
					argList->recurisve = true;
					break;
					
				case 'l':
					argList->long_format = true;
					break;
				
				case '@':
					argList->extended_attributes = true;
					break;
				
				case 'S':
					if (argList->sort_time)
						argList->sort_time = false;
					argList->sort_size = true;
					break;

				case 's':
					argList->block_size = true;
					break;

				case 'f':
					argList->not_sort = true;
					argList->all = true;
					argList->sort_time = false;
					argList->sort_size = false;
					argList->reverse = false;
					break;
				
				case 'g':
					argList->no_name = true;
					argList->long_format = true;
					break;
				
				case 'n':
					argList->long_format = true;
					argList->id = true;
					break;
					
				default:
					usage(option);
					return EXIT_FAILURE;
					break;
			}
		}
	}

	//Retrieve all path
	if (getAllPath(ac, av, argList, i) == EXIT_FAILURE)
		return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}