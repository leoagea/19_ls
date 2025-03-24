/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:23:49 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 22:02:44 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void usage(char invalidOption)
{
	ft_printf(2, "ls: invalid option -- %c\n", invalidOption);
	ft_printf(2, "usage: ls [-alrtR] [file ...]\n");	
}

void freeArgStruct(t_arg *argList)
{
	if (argList->path){
		free(argList->path);
		argList->path = NULL;
	}
}
