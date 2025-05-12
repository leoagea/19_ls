/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:21:54 by lagea             #+#    #+#             */
/*   Updated: 2025/05/12 23:52:51 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_LS_H__
# define __FT_LS_H__


/*#############################################################################
# Includes
#############################################################################*/

# include "../libft/libft.h"
# include "struct.h"
# include "define.h"

# include <stdlib.h>	// exit, EXIT_FAILURE
# include <stdio.h>		// perror
# include <stdlib.h>	// exit, EXIT_FAILURE
# include <dirent.h>	// DIR, struct dirent, opendir, readdir, closedir
# include <time.h>      // time_t, ctime
# include <unistd.h>    //readlink
# include <pwd.h>       //getpwuid
# include <grp.h>       //getgrgid
# include <sys/ioctl.h>	// ioctl
# include <sys/xattr.h>

/*#############################################################################
# Init functions
#############################################################################*/

void    initArgStruct(t_arg *argStruct);
void    initFormatStruct(t_format *format);
void    initInfoStruct(t_info *info);

/*#############################################################################
# Debug functions
#############################################################################*/

void    print_argList(t_arg argList);
void    print_dirent(struct dirent *entry);
void    printFormatStruct(t_format *format);
void    printInfoStruct(t_info *info);
void    printNodeLs(t_ls *node);
void    print_subdir(void *content);
void	print_colors_map(t_color_map *colors);

/*#############################################################################
# ParseArg functions
#############################################################################*/

int     parseArg(int ac, char **av, t_arg *argList);

/*#############################################################################
# Explore functions
#############################################################################*/

// int     explore_loop(t_arg arglist);
int handleSymlink(t_ls *node);
int explore_loop(t_data *data);
int exploreDirectories(t_data *data, t_dll *list, char *path);
// int     exploreDirectories(t_arg argList, char *path);

/*#############################################################################
# RetrieveInfo functions
#############################################################################*/

// int     retrieveAllInfo(t_ls_node *node, t_arg arg, char *path, struct dirent *entry, t_format *format);
int retrieveAllInfo(t_data *data, t_ls *node);

/*#############################################################################
# FormatOutput functions
#############################################################################*/

void appendStr(char *str, char *append);
void appendChar(char *str, char c);
void getFormatLen(t_ls *node, t_format *format);
void formatLongFormat(t_ls *node, t_format *format);
void formatOther(t_ls *node);
void formatOutput(t_ls *node, t_arg arg);

/*#############################################################################
# ConsoleOutput functions
#############################################################################*/

// void    output(t_dll *list, t_arg arg);
void output(t_data *data, t_dll *list);
void print_recursive(t_data *data, t_dll *list);

/*#############################################################################
# Colors functions
#############################################################################*/

void init_colors(t_data *data);
char *get_color_from_env(t_ls *ls, t_data *data);

/*#############################################################################
# Utils functions
#############################################################################*/

void    usage(char invalidOption);
void    help(void);
char    *extractTimeModified(struct stat info);
void    extractPerm(char *perm, int mode);
int     compareName(void *a, void *b);
int     compareTime(void *a, void *b);
int     compareSubdirName(void *a, void *b);
int		compareSize(void *a, void *b);
int     calculateTotalBlocks(t_dll *list);
t_ls *mallocLs(t_format *format);
t_subdir *mallocSubdir(void);
char *get_color_for_file(t_ls *ls);

/*#############################################################################
# Free functions
#############################################################################*/

void	free2Array(char **array);
void    freeArgStruct(t_arg *argList);
void    freeLsNode(void *content);
void    freeStr(char **str);
void 	freeVoid(void **content);
void    freeList(void *content);
void  	freeFormatStruct(t_format **format);
void	freeXattr(void *content);

#endif