/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/05/14 00:34:48 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
	t_data data;
	data.is_tty = isatty(STDOUT_FILENO);
	initArgStruct(&data.arg);
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &data.w);
	// print_terminal_size(&data.w); // For debugging
	init_colors(&data);
	// print_colors_map(&data.colors); // For debugging

	t_dll list;
	dll_init(&list);
	data.list = &list;

	int parse_result = parseArg(ac, av, &data.arg);
	if (parse_result == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (parse_result == EXIT_CLEAN)
		return EXIT_SUCCESS;

	// print_argList(data.arg);
	explore_loop(&data);
	freeArgStruct(&data.arg);
	dll_free(data.list, freeList);
	// system("leaks ft_ls");

	return EXIT_SUCCESS;
}
