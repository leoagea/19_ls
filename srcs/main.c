/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/03/24 22:16:59 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
	t_arg argList; 
	t_dll list;
	
	initArgStruct(&argList);
	dll_init(&list);
	if (ac != 1){
		if (parseArg(ac - 1, av + 1, &argList))
			return EXIT_FAILURE;
	}
	// print_argList(argList);
	explore_loop(&list, argList);
	freeArgStruct(&argList);
	return EXIT_SUCCESS;
}