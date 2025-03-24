/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:36:26 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 22:08:05 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void print_argList(t_arg argList)
{
	ft_printf(2, "Flags: \n");
	ft_printf(2, "\tall: %b\n", argList.all);
	ft_printf(2, "\treverse: %b\n", argList.reverse);
	ft_printf(2, "\tsort_time: %b\n", argList.sort_time);
	ft_printf(2, "\trecursive: %b\n", argList.recurisve);
	ft_printf(2, "\tlong_format: %b\n", argList.long_format);
	ft_printf(2, "\tpath: %s\n", argList.path);
	ft_printf(2, "\tall_path: \n");
	int i = 0;
	while (argList.all_path[i] != NULL){
		ft_printf(2, "\t\t%s\n", argList.all_path[i]);
		i++;
	}
}

void	dll_print_backward(t_dll *dll)
{
	t_node	*current;

	current = dll->tail;
	while (current != NULL)
	{
		// ft_printf("value : %s\n", current->str);
		current = current->prev;
	}
}

void	dll_print_forward(t_dll *dll)
{
	t_node	*current;

	current = dll->head;
	while (current != NULL)
	{
		print_dirent(current->content);
		current = current->next;
	}
}

void print_dirent(struct dirent *entry)
{
	ft_printf(1, "name: %s\n", entry->d_name);
}