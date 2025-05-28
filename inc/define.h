/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:54:22 by lagea             #+#    #+#             */
/*   Updated: 2025/05/28 13:50:47 by lagea            ###   ########.fr       */
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
#define EXIT_ERROR	 2
#define EXIT_CLEAN	 3

/*#############################################################################
# Text
#############################################################################*/

#define DESCIPTION \
	"\
List information about the FILEs (the current directory by default).\n\
Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n\n"
#define OPTIONS "Options:\n"
#define AFLAG \
"\t-a	do not ignore entries starting with .\n"
#define RFLAG \
"\t-r	reverse order while sorting\n\
\t-R	list subdirectories recursively\n"
#define TFLAG \
"\t-t	sort by time, newest first\n"
#define LFLAG \
"\t-l	use a long listing format\n"
#define EXTFLAG \
"\t-@	list extended attributes\n"
#define SIZEFLAG \
"\t-S	sort by file size, largest first\n\
\t-s	print the allocated size of each file, in blocks\n"
#define FFLAG \
"\t-f	do not sort, enable -a, disable -ls\n"
#define GFLAG \
"\t-g	same as -l, but do not list owner\n\
\t-n	same as -l, but list numeric user and group IDs\n"
#define SLASHFLAG \
"\t-p	add a slash after each directory name\n"
#define ONEEFLAG \
"\t-1	list one file per line\n"
#define COMMAFLAG \
"\t-,	list entries separated by commas\n"
#define MFLAG \
"\t-m	list entries horizontally\n"
#define UFLAG \
"\t-u	sort by last access time instead of last modification time\n"
#define DFLAG \
"\t-d	do not list subdirectories, only the directories themselves\n"
#define HFLAG \
"\t-h	print sizes in human readable format (e.g., 1K, 234M, 2G)\n"
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