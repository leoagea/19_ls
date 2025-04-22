/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consoleOutput.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:44:36 by lagea             #+#    #+#             */
/*   Updated: 2025/04/22 18:14:50 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

static void print_recursive(t_data *data, t_dll *list)
{
    if (!list) {
        printf("Error: NULL list passed to print_direct\n");
        return;
    }

    t_dll *printsubdir = malloc(sizeof(t_dll));
    if (!printsubdir) {
        printf("Error: Failed to allocate printsubdir\n");
        return;
    }
    dll_init(printsubdir);
    
    if (data->arg.long_format)
       ft_printf(1, "%s %d\n", TOTAL_BLOCKS, calculateTotalBlocks(list));
       
    t_node *node = list->head;
    while (node != NULL) {
        if (!node->content) {
            printf("Warning: Skipping NULL content node\n");
            node = node->next;
            continue;
        }

        t_ls *ls = node->content;
        ft_printf(1, "%s\n", ls->format);
        
        // printf("ls->name: %s\n", ls->name);
        if (ls->is_dir && ls->subdir){
            t_subdir *subdir = mallocSubdir();
            if (!subdir) {
                printf("Error: Failed to allocate subdir\n");
                node = node->next;
                continue;
            }
            subdir->name = ft_strdup(ls->name);
            subdir->path = ft_strdup(ls->relative_path);
            subdir->subdir_list = ls->subdir;
            dll_insert_tail(subdir, printsubdir);
        }
        node = node->next;
    } 
    
    if (printsubdir->head == NULL) {
        free(printsubdir);
        return;
    }
    dll_bubble_sort(printsubdir->head, printsubdir->tail, compareSubdirName);
    if (data->arg.reverse)
        dll_revert(printsubdir);
    // dll_print_forward(printsubdir, print_subdir);
    
    t_node *current = printsubdir->head;
    while (current) {
        t_subdir *tmp = current->content;
        if (tmp && tmp->subdir_list) {
            ft_printf(1, "\n%s:\n", tmp->path);
            print_recursive(data, tmp->subdir_list);
        }
        current = current->next;
    }
    
}

static void print_direct(t_data *data, t_dll *list)
{
    if (!list) {
        printf("Error: NULL list passed to print_direct\n");
        return;
    }

    if (data->arg.long_format)
        ft_printf(1, "%s %d\n", TOTAL_BLOCKS, calculateTotalBlocks(list));
    
    t_node *node = list->head;
    while (node != NULL) {
        if (!node->content) {
            printf("Warning: Skipping NULL content node\n");
            node = node->next;
            continue;
        }

        t_ls *ls = node->content;
        ft_printf(1, "%s\n", ls->format);
        node = node->next;
    }
}

void output(t_data *data, t_dll *list)
{
    if (list->head == NULL){
        printf("null output\n");
        return;
    }
    if (data->arg.sort_time)
        dll_bubble_sort(list->head, list->tail, compareTime);
    else
        dll_bubble_sort(list->head, list->tail, compareName);
        
    if (data->arg.reverse)
        dll_revert(list);
    // else
    //     dll_print_forward(list, print);

    // t_dll *dll = list->head->content;
    // t_node *node = list->head;
    // while (node != NULL) {
    //     print_direct(data, list);
    //     node = node->next;
    // }

    if (data->arg.recurisve)
        print_recursive(data, list);
    else
        print_direct(data, list);
}
