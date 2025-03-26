/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/03/26 23:35:46 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
	t_arg argList; 
	
	initArgStruct(&argList);
	if (ac != 1){
		if (parseArg(ac - 1, av + 1, &argList))
			return EXIT_FAILURE;
	}
	// print_argList(argList);
	explore_loop(argList);
	freeArgStruct(&argList);
	// system("leaks ft_ls");
	return EXIT_SUCCESS;
}