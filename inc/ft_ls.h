/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:21:54 by lagea             #+#    #+#             */
/*   Updated: 2025/04/17 18:56:31 by lagea            ###   ########.fr       */
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
    #define TOTAL_BLOCKS "total:"
# elif __linux__
    #define TOTAL_BLOCKS "total"
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
void    initFormatStruct(t_format *format);

/*#############################################################################
# Debug functions
#############################################################################*/

void    print_argList(t_arg argList);
void    printAllElementsLsNode(t_ls_node *node);
void    print_dirent(struct dirent *entry);
void    printFormatStruct(t_format *format);
void printNodeLs(t_ls *node);

/*#############################################################################
# ParseArg functions
#############################################################################*/

int     parseArg(int ac, char **av, t_arg *argList);

/*#############################################################################
# Explore functions
#############################################################################*/

// int     explore_loop(t_arg arglist);
int explore_loop(t_data *data);
int exploreDirectories(t_data *data, t_dll *list, char *path);
// int     exploreDirectories(t_arg argList, char *path);

/*#############################################################################
# RetrieveInfo functions
#############################################################################*/

// int     retrieveAllInfo(t_ls_node *node, t_arg arg, char *path, struct dirent *entry, t_format *format);
int retrieveAllInfo(t_data *data, t_ls *node);
int     retrieveSymInfo(t_ls_node *node, t_arg arg);

/*#############################################################################
# FormatOutput functions
#############################################################################*/

void appendStr(char *str, char *append);
void appendChar(char *str, char c);
void getFormatLen(t_ls *node, t_format *format);
void formatLongFormat(t_ls_node *node, t_format *format);
void formatOther(t_ls_node *node);
void formatOutput(t_ls_node *node, t_arg arg, t_format *format);

/*#############################################################################
# ConsoleOutput functions
#############################################################################*/

// void    output(t_dll *list, t_arg arg);
void output(t_data *data, t_dll *list);
void    print(void *content);


/*#############################################################################
# Utils functions
#############################################################################*/

void    usage(char invalidOption);
void    freeArgStruct(t_arg *argList);
char    *extractTimeModified(struct stat info);
void    extractPerm(char *perm, int mode);
int     compareName(void *a, void *b);
int     compareTime(void *a, void *b);
t_ls_node *mallocLsNode(void);
int     calculateTotalBlocks(t_dll *list);
int ft_max(int a, int b);
t_ls *mallocLs();

#endif