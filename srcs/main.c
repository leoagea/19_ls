/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/05/19 17:13:53 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
	t_data data;
	data.is_tty = isatty(STDOUT_FILENO);
	initArgStruct(&data.arg);
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &data.w);
	// debug_print_terminal_size(&data.w); // For debugging
	init_colors(&data);
	// debug_print_colors_map(&data.colors); // For debugging

	t_dll list;
	dll_init(&list);
	data.list = &list;

	int parse_result = parseArg(ac, av, &data);
	if (parse_result == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (parse_result == EXIT_CLEAN)
		return EXIT_SUCCESS;

	// debug_print_argList(data.arg);
	explore_loop(&data);
	freeAll(&data);
	// system("leaks ft_ls");

	return EXIT_SUCCESS;
}
