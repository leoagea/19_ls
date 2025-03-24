/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:54:12 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 19:34:24 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void initArgStruct(t_arg *argStruct)
{
	argStruct->path = NULL;
	argStruct->all_path = NULL;
	argStruct->all = false;
	argStruct->reverse = false;
	argStruct->sort_time = false;
	argStruct->recurisve = false;
	argStruct->long_format = false;
}