/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consoleOutput.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:44:36 by lagea             #+#    #+#             */
/*   Updated: 2025/03/26 19:27:32 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

void output(t_dll *list, t_arg arg)
{
    if (arg.long_format)
        printLongFormat(list, arg);
    else
        printShortFormat(list);
}

void printShortFormat(t_dll *list)
{
    int size_dll = dll_size(list);
    if (size_dll == 0)
        return;
    
    t_node *current = list->head;
    while (current != NULL)
    {
        t_ls_node *node = current->content;
        ft_printf(1, "%s\n", node->name);
        current = current->next;
    }
}

void printLongFormat(t_dll *list, t_arg arg)
{
    int size_dll = dll_size(list);
    if (size_dll == 0)
        return;
    
    if (!arg.sort_time){
        dll_bubble_sort(list->head, list->tail, compareName);
    }
    
    t_node *current = list->head;
    while (current != NULL)
    {
        t_ls_node *node = current->content;
        // if (size_dll > 1)
        //     ft_printf(1, "%s:\n", node->relative_path);
        if (node->type == DIRECTORY)
            ft_printf(1, "d");
        else if (node->type == FILE)
            ft_printf(1, "-");
        else if (node->type == LINK)
            ft_printf(1, "l");
        else
            ft_printf(1, "u");
        ft_printf(1, "%s %d %s %s %d %s %s", node->perm, node->nlink, node->user_name, node->group_name, node->size_bytes, node->last_mod, node->name);
        if (node->symbolic)
            ft_printf(1, " -> %s", node->sym_name);
        ft_printf(1, "\n");
        current = current->next;
    }
}