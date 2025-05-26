/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/05/26 16:19:31 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ls.h"

int main(int ac, char **av)
{
	t_data data;
	data.return_val = EXIT_SUCCESS;
	initArgStruct(&data.arg);
	data.is_tty = isatty(STDOUT_FILENO);
	if (!data.is_tty)
		data.arg.oneline = true;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &data.w);
	// debug_print_terminal_size(&data.w); // For debugging
	init_colors(&data);
	// debug_print_colors_map(&data.colors); // For debugging

	t_dll list;
	dll_init(&list);
	data.list = &list;

	int parse_result = parseArg(ac, av, &data);
	if (parse_result == EXIT_FAILURE)
		return (freeAll(&data), EXIT_FAILURE);
	if (parse_result == EXIT_CLEAN)
		return (freeAll(&data), EXIT_SUCCESS);

	// debug_print_argList(data.arg);
	explore_loop(&data);
	freeAll(&data);
	// system("leaks ft_ls");

	return data.return_val;
}
