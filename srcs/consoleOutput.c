/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consoleOutput.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:44:36 by lagea             #+#    #+#             */
/*   Updated: 2025/04/21 16:37:55 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void print_direct(t_data *data, t_dll *list)
{
    t_dll subdir;
    dll_init(&subdir);
    
    t_node *node = list->head;
    while (node != NULL) {
        t_ls *ls = node->content;
        ft_printf(1, "%s\n", ls->format);
        // if (ls->type == DIRECTORY){
        //     dll_insert_tail(ls->subdir, &subdir);
        // }
        node = node->next;
    } 
    (void)data;
    // if (subdir.head != NULL){
    //     ft_printf(1, "\n");
    //     output(data, &subdir);
    // }
}

void output(t_data *data, t_dll *list)
{
    if (list->head == NULL){
        printf("null output\n");
        return;
    }
    // if (arg.sort_time)
    //     dll_bubble_sort(list->head, list->tail, compareTime);
    // else
    //     dll_bubble_sort(list->head, list->tail, compareName);
        
    // if (arg.long_format)
    //    ft_printf(1, "%s %d\n", TOTAL_BLOCKS, calculateTotalBlocks(list));
        
    // if (arg.reverse)
    //     dll_print_backward(list, print);
    // else
    //     dll_print_forward(list, print);

    // t_dll *dll = list->head->content;
    // t_node *node = list->head;
    // while (node != NULL) {
    //     print_direct(data, list);
    //     node = node->next;
    // }
    print_direct(data, list);

}

void print(void *content)
{
    t_ls_node *node = content;

	ft_printf(1, "%s\n", node->format);
}