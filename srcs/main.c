/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/04/17 15:45:04 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
	t_data data;
	
	t_arg argList; 
	initArgStruct(&argList);
	if (ac != 1){
		if (parseArg(ac - 1, av + 1, &argList))
			return EXIT_FAILURE;
	}
	data.arg = argList;
	t_dll list;
	dll_init(&list);
	data.list = &list;
	// print_argList(argList);
	explore_loop(&data);
	freeArgStruct(&argList);
	// system("leaks ft_ls");
	return EXIT_SUCCESS;
}