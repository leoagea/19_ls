/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consoleOutput.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:44:36 by lagea             #+#    #+#             */
/*   Updated: 2025/03/27 00:43:42 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void output(t_dll *list, t_arg arg)
{
    if (arg.sort_time)
        dll_bubble_sort(list->head, list->tail, compareTime);
    else
        dll_bubble_sort(list->head, list->tail, compareName);
    if (arg.reverse)
        dll_print_backward(list, print);
    else
        dll_print_forward(list, print);
}

void print(void *content)
{
    t_ls_node *node = content;
    
	ft_printf(1, "%s\n", node->format);
}