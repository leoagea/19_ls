/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:21:54 by lagea             #+#    #+#             */
/*   Updated: 2025/05/26 16:47:02 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_LS_H__
#define __FT_LS_H__

/*#############################################################################
# Includes
#############################################################################*/

#include "../libft/libft.h"
#include "struct.h"
#include "define.h"

#include <stdlib.h>	   // exit, EXIT_FAILURE
#include <stdio.h>	   // perror
#include <stdlib.h>	   // exit, EXIT_FAILURE
#include <dirent.h>	   // DIR, struct dirent, opendir, readdir, closedir
#include <time.h>	   // time_t, ctime
#include <unistd.h>	   //readlink
#include <pwd.h>	   //getpwuid
#include <grp.h>	   //getgrgid
#include <sys/ioctl.h> // ioctl
#include <sys/xattr.h>
#include <sys/sysmacros.h> // major, minor
#include <locale.h> // setlocale

/*#############################################################################
# Init functions
#############################################################################*/

void initArgStruct(t_arg *argStruct);
void initFormatStruct(t_format *format);
void initInfoStruct(t_info *info);
t_ls	 *mallocLs();
t_subdir *mallocSubdir(void);

/*#############################################################################
# Debug functions
#############################################################################*/

void debug_print_argList(t_arg argList);
void debug_print_dirent(struct dirent *entry);
void debug_printFormatStruct(t_format *format);
void debug_printInfoStruct(t_info *info);
void debug_printNodeLs(t_ls *node);
void debug_print_subdir(void *content);
void debug_print_colors_map(t_color_map *colors);
void debug_print_terminal_size(struct winsize *w);

/*#############################################################################
# ParseArg functions
#############################################################################*/

int parseArg(int ac, char **av, t_data *data);

/*#############################################################################
# Explore functions
#############################################################################*/

// int     explore_loop(t_arg arglist);
int handleSymlink(t_ls *node);
int explore_loop(t_data *data);
int processEntry(t_data *data, t_ls *node, t_format **format);
int exploreDirectories(t_data *data, t_dll *list, char *path);
// int     exploreDirectories(t_arg argList, char *path);

/*#############################################################################
# RetrieveInfo functions
#############################################################################*/

int retrieveAllInfo(t_data *data, t_ls *node, t_format **format);
void	  extractPerm(char *perm, int mode);

/*#############################################################################
# FormatOutput functions
#############################################################################*/

void appendStr(char *str, char *append);
void appendChar(char *str, char c);
void appendInt(char *str, int num);
void getFormatLen(t_ls *node, t_format **format);
void formatLongFormat(t_arg arg, t_ls *node, t_format *format);
void formatOther(t_arg arg, t_ls *node, t_format *format);
void formatOutput(t_format *format, t_ls *node, t_arg arg);

/*#############################################################################
# ConsoleOutput functions
#############################################################################*/

// void    output(t_dll *list, t_arg arg);
void print_format(t_data *data, t_ls *ls);
void output(t_data *data, t_dll *list);
void print_recursive(t_data *data, t_dll *list);

/*#############################################################################
# Colors functions
#############################################################################*/

void  init_colors(t_data *data);
char *get_color_from_env(t_ls *ls, t_data *data);

/*#############################################################################
# Sort functions
#############################################################################*/

int		  compareName(void *a, void *b);
int		  compareTime(void *a, void *b);
int		  compareSubdirName(void *a, void *b);
int		  compareSize(void *a, void *b);

/*#############################################################################
# Utils functions
#############################################################################*/

void	  usage(char invalidOption);
void	  help(void);
int		  calculateTotalBlocks(t_dll *list);
char	 *get_color_for_file(t_ls *ls);
char	 *int_to_str_sep(t_ls *node, char *num);
char     *string_to_lower(char *str);
int		  get_max_len(t_dll *list);

/*#############################################################################
# Free functions
#############################################################################*/

void free2Array(char **array);
void freeArgStruct(t_arg *argList);
void freeLsNode(void *content);
void freeStr(char **str);
void freeVoid(void **content);
void freeList(void *content);
void freeFormatStruct(t_format **format);
void freeXattr(void *content);
void freeColorMap(t_color_map *colors);
void freeAll(t_data *data);
void freeSubdir(void *content);

#endif