/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:54:22 by lagea             #+#    #+#             */
/*   Updated: 2025/05/15 21:40:12 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DEFINE_H_
#define _DEFINE_H_

/*#############################################################################
# Variables
#############################################################################*/

#ifdef __APPLE__
#	define st_mtime				 st_mtimespec.tv_sec
#	define CALC_BLOCKS(blocks) block
#	define GET_XATTR(path, name, value, size) getxattr(path, name, value, size, 0, XATTR_NOFOLLOW)
#	define LIST_XATTR(path, list, size) listxattr(path, list, size, XATTR_NOFOLLOW)
#elif __linux__
#	define CALC_BLOCKS(blocks) (blocks / 2)
#	define GET_XATTR(path, name, value, size) getxattr(path, name, value, size)
#   define LIST_XATTR(path, list, size) listxattr(path, list, size)
#endif

#define ENV_LSCOLORS "LS_COLORS"
#define TOTAL_BLOCKS "total"
#define DEFAULT_PATH "."
#define EXIT_CLEAN	 2

/*#############################################################################
# Text
#############################################################################*/

#define DESCIPTION \
	"\
List information about the FILEs (the current directory by default).\n\
Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n\n"
#define OPTIONS "Options:\n"
#define AFLAG \
	"\t-a, --all                  do not ignore entries starting with .\n"
#define RFLAG \
	"\t-r, --reverse              reverse order while sorting\n\
\t-R, --recursive            list subdirectories recursively\n"
#define TFLAG "\t-t, --sort=time            sort by time, newest first\n"
#define LFLAG "\t-l, --long                 use a long listing format\n"
#define EXIT \
	"\nExit status:\n\
\t0  if OK,\n\
\t1  if minor problems (e.g., cannot access subdirectory),\n\
\t2  if serious trouble (e.g., cannot access command-line argument).\n"

/*#############################################################################
# Colors
#############################################################################*/

#define COLOR_RESET	 "\033[0m"
#define COLOR_DIR	 "\033[1;36m"  // Bold cyan for directories
#define COLOR_LINK	 "\033[35m"	   // Magenta for symbolic links
#define COLOR_SOCKET "\033[32m"	   // Green for sockets
#define COLOR_PIPE	 "\033[33m"	   // Yellow for pipes
#define COLOR_EXEC	 "\033[31m"	   // Red for executables
#define COLOR_BLOCK	 "\033[34;46m" // Blue on cyan for block devices
#define COLOR_CHAR	 "\033[34;43m" // Blue on yellow for character devices

#endif