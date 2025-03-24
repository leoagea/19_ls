/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseArg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:30:57 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 19:38:07 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int parseArg(int ac, char **av, t_arg *argList)
{
	int i = -1;
	
	//Parse all options
	while (++i < ac && av[i][0] == '-'){
		int j = 0;
		
		while (av[i][++j]){
			char option = av[i][j];
			
			switch (option){
				case 'a':
					argList->all = true;
					break;
				
				case 'r':
					argList->recurisve = true;
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
					
				default:
					usage(option);
					return 1;
					break;
			}
		}
	}

	//Retrieve all path
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
			return 0;
		argList->all_path[0] = malloc(2 * sizeof(char));
		argList->all_path[0] = DEFAULT_PATH;
		argList->all_path[1] = NULL;
	}
	else{
		argList->all_path = ft_split(argList->path, ' ');
	}
	return 0;
}