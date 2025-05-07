/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/05/07 23:44:32 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
    t_data data;
    initArgStruct(&data.arg);
    dll_init(data.list);
    
    int parse_result = parseArg(ac, av, &data.arg);
    if (parse_result == EXIT_FAILURE)
        return EXIT_FAILURE;
    if (parse_result == EXIT_CLEAN)
        return EXIT_SUCCESS;

    explore_loop(&data);
    freeArgStruct(&data.arg);
    dll_free(data.list, freeList); 
    // system("leaks ft_ls");
    return EXIT_SUCCESS;
}
