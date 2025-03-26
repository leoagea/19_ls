/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:21:54 by lagea             #+#    #+#             */
/*   Updated: 2025/03/26 22:33:44 by lagea            ###   ########.fr       */
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
# include <time.h>      // time_t, ctime
# include <unistd.h>    //readlink
# include <pwd.h>        //getpwuid
# include <grp.h>        //getgrgid

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

void    initArgStruct(t_arg *argStruct);
void    initLsNode(t_ls_node *node);

/*#############################################################################
# Debug functions
#############################################################################*/

void    print_argList(t_arg argList);
void    printAllElementsLsNode(t_ls_node *node);
void    print_dirent(struct dirent *entry);

/*#############################################################################
# ParseArg functions
#############################################################################*/

int     parseArg(int ac, char **av, t_arg *argList);

/*#############################################################################
# Explore functions
#############################################################################*/

int     explore_loop(t_dll *list, t_arg arglist);
int     exploreDirectories(t_arg argList, t_dll *list, char *path);
t_ls_node *newLsNode(t_arg arg, char *path, struct dirent *entry);

/*#############################################################################
# ConsoleOutput functions
#############################################################################*/

void    output(t_dll *list, t_arg arg);
void    printShortFormat(t_dll *list);
void    printLongFormat(t_dll *list, t_arg arg);

/*#############################################################################
# Utils functions
#############################################################################*/

void    usage(char invalidOption);
void    freeArgStruct(t_arg *argList);
char    *extractTimeModified(struct stat info);
void    extractPerm(char *perm, int mode);
int     compareName(void *a, void *b);

#endif