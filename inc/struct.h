/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:32:56 by lagea             #+#    #+#             */
/*   Updated: 2025/04/22 18:09:00 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
# define __STRUCT_H__

# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>

typedef struct s_arg
{
	bool	all; //-a
	bool	reverse; //-r
	bool	sort_time; //-t
	bool	recurisve; //-R
	bool	long_format; //-l
	char*	path;
	char**	all_path;
}				t_arg;

typedef struct s_format
{
	int 		max_user;
	int 		max_group;
	int 		max_link;
	int 		max_size_bytes;
}				t_format;	

typedef struct s_info
{
	// Long Format Info
	int				size_bytes;
	char			sym_name[256];
	char*			group_name;
	char*			user_name;
	char			perm[10];
	char*			last_mod;
	int 			nlink;
	
	// Len Info
	int				nlink_len;
	int 			user_name_len;
	int 			group_name_len;
	int 			size_bytes_len;
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
}				t_data;

#endif