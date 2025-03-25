/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:32:56 by lagea             #+#    #+#             */
/*   Updated: 2025/03/25 01:41:07 by lagea            ###   ########.fr       */
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
	char			*name;
	char			*last_mod;
	bool			symbolic;
	char			*sym_name;
	struct stat		*info;
	struct dirent 	*entry;
}				t_ls_node;

#endif