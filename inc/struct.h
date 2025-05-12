/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:32:56 by lagea             #+#    #+#             */
/*   Updated: 2025/05/13 00:03:14 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
# define __STRUCT_H__

# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>	// struct winsize

enum e_type
{
	UNKNOWN = 0,
	FIFO = 1,
	CHARFILE = 2, //Character special file
	DIRECTORY = 4,
	BLKFILE = 6, //Block special file
	REGFILE = 8,
	LINK = 10,
	SOCKET = 12,
	CFS = 14, //Network file system
};

// Color mapping structure
typedef struct s_color_map {
    char *di; 	// directory
    char *ln; 	// symlink
    char *so; 	// socket
    char *pi; 	// pipe
    char *ex; 	// executable
    char *bd; 	// block device
    char *cd; 	// char device
    char *fi; 	// regular file
} 				t_color_map;

typedef struct s_xattr {
    char    *name;          // Attribute name
    void    *value;         // Attribute value
    ssize_t value_size;     // Size of value
    struct s_xattr *next;   // Next attribute
} 				t_xattr;

typedef struct s_arg
{
	bool	all; //-a
	bool	reverse; //-r
	bool	sort_time; //-t
	bool	recurisve; //-R
	bool	long_format; //-l
	bool	extended_attributes; //-@
	bool	sort_size; //-S
	bool	block_size; //-s
	char*	path;
	char**	all_path;
}				t_arg;

typedef struct s_format
{
	int 		max_user;
	int 		max_group;
	int 		max_link;
	int 		max_size_bytes;
	int			max_name;
	int			max_block_size;
}				t_format;	

typedef struct s_info
{
	// Long Format Info
	int				size_bytes;
	char			sym_name[256];
	char*			group_name;
	char*			user_name;
	char			perm[11];
	char*			last_mod;
	int 			nlink;
	int 			block_size;
	
	// Len Info
	int				nlink_len;
	int 			user_name_len;
	int 			group_name_len;
	int 			size_bytes_len;
	int				name_len;
	int 			block_size_len;
}			t_info;

typedef struct s_ls
{
	char *relative_path;
	char *name;
	int type;
	bool is_symbolic;
	bool is_dir;
	char format[256];

	t_info *info;
	t_format *format_info;
	t_dll *subdir;
	t_dll *xattr_list;
}				t_ls;

typedef struct s_subdir
{
	char *name;
	char *path;
	t_dll *subdir_list;
}				t_subdir;

typedef struct s_data
{
	t_dll *list;
	t_arg arg;
	struct winsize w;
	t_color_map colors;
	bool use_color;
	bool is_tty;
}				t_data;

#endif