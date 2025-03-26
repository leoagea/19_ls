/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:32:56 by lagea             #+#    #+#             */
/*   Updated: 2025/03/27 00:20:15 by lagea            ###   ########.fr       */
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

typedef struct s_ls_node
{
	char*			relative_path;
	int				type;
	char*			name;
	bool			symbolic;
	char			format[256];
	
	// Long Format Info
	int				size_bytes;
	char			sym_name[256];
	char*			group_name;
	char*			user_name;
	char			perm[10];
	char*			last_mod;
	int 			nlink;
	
	struct stat		*info;
	struct dirent 	*entry;
}				t_ls_node;

#endif