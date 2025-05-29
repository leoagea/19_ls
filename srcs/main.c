/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:02 by lagea             #+#    #+#             */
/*   Updated: 2025/05/29 15:28:35 by lagea            ###   ########.fr       */
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
	init_colors(&data);

	t_dll list;
	dll_init(&list);
	data.list = &list;

	int parse_result = parseArg(ac, av, &data);
	if (parse_result == EXIT_FAILURE)
		return (freeAll(&data), EXIT_FAILURE);
	if (parse_result == EXIT_CLEAN)
		return (freeAll(&data), EXIT_SUCCESS);

	explore_loop(&data);
	freeAll(&data);

	return data.return_val;
}
