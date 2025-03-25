/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:21:54 by lagea             #+#    #+#             */
/*   Updated: 2025/03/25 18:31:38 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_LS_H__
# define __FT_LS_H__


/*#############################################################################
# Includes
#############################################################################*/
# include "../libft/libft.h"
# include "struct.h"
# include <stdlib.h>	// exit, EXIT_FAILURE
# include <stdio.h>		// perror
# include <stdlib.h>	// exit, EXIT_FAILURE
# include <dirent.h>	// DIR, struct dirent, opendir, readdir, closedir
# include <time.h>       // time_t, ctime

/*#############################################################################
# Defines
#############################################################################*/

# ifdef __APPLE__
    #define st_mtime st_mtimespec.tv_sec
# endif

# define DEFAULT_PATH "."
# define UNKNOWN 0
# define DIRECTORY 4
# define FILE 8
# define LINK 10

/*#############################################################################
# Init functions
#############################################################################*/

void initArgStruct(t_arg *argStruct);

/*#############################################################################
# Debug functions
#############################################################################*/

void print_argList(t_arg argList);
void dll_print_backward(t_dll *dll);
void dll_print_forward(t_dll *dll);
void printAllElementsLsNode(t_ls_node *node);
void print_dirent(struct dirent *entry);

/*#############################################################################
# ParseArg functions
#############################################################################*/

int parseArg(int ac, char **av, t_arg *argList);

/*#############################################################################
# Explore functions
#############################################################################*/

int explore_loop(t_dll *list, t_arg arglist);
int exploreDirectories(t_arg argList, t_dll *list, char *path);

/*#############################################################################
# Utils functions
#############################################################################*/

void usage(char invalidOption);
void freeArgStruct(t_arg *argList);
t_ls_node *newLsNode(char *path, struct dirent *entry);
char *extractTimeModified(struct stat info);

#endif