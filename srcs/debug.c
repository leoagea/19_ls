/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:36:26 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 19:39:59 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void print_argList(t_arg argList)
{
	ft_printf("Flags: \n");
	ft_printf("\tall: %b\n", argList.all);
	ft_printf("\treverse: %b\n", argList.reverse);
	ft_printf("\tsort_time: %b\n", argList.sort_time);
	ft_printf("\trecursive: %b\n", argList.recurisve);
	ft_printf("\tlong_format: %b\n", argList.long_format);
	ft_printf("\tpath: %s\n", argList.path);
	ft_printf("\tall_path: \n");
	int i = 0;
	while (argList.all_path[i] != NULL){
		ft_printf("\t\t%s\n", argList.all_path[i]);
		i++;
	}
}