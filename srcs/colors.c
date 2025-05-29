#include "../inc/ft_ls.h"

static void parse_ls_colors(char *ls_colors, t_color_map *colors)
{
	char **token = ft_split(ls_colors, ':');
	char **tmp = token;
	while (*token) {
		if (ft_strncmp(*token, "di=", 3) == 0)
			colors->di = ft_strdup(*(token) + 3);
		else if (ft_strncmp(*token, "ln=", 3) == 0)
			colors->ln = ft_strdup(*(token) + 3);
		else if (ft_strncmp(*token, "so=", 3) == 0)
			colors->so = ft_strdup(*(token) + 3);
		else if (ft_strncmp(*token, "pi=", 3) == 0)
			colors->pi = ft_strdup(*(token) + 3);
		else if (ft_strncmp(*token, "ex=", 3) == 0)
			colors->ex = ft_strdup(*(token) + 3);
		else if (ft_strncmp(*token, "bd=", 3) == 0)
			colors->bd = ft_strdup(*(token) + 3);
		else if (ft_strncmp(*token, "cd=", 3) == 0)
			colors->cd = ft_strdup(*(token) + 3);
		token++;
	}

	free2Array(tmp);
}

void init_colors(t_data *data)
{
	char *ls_colors = getenv(ENV_LSCOLORS);
	data->use_color = isatty(STDOUT_FILENO);

	memset(&data->colors, 0, sizeof(t_color_map));
	if (!ls_colors || !data->use_color) {
		data->use_color = false;
		return;
	}

	parse_ls_colors(ls_colors, &data->colors);
}

char *get_color_from_env(t_ls *ls, t_data *data)
{
	if (!data->use_color)
		return "";

	const char *code = NULL;

	switch (ls->type) {
		case DIRECTORY:
			code = data->colors.di;
			break;
		case LINK:
			code = data->colors.ln;
			break;
		case SOCKET:
			code = data->colors.so;
			break;
		case FIFO:
			code = data->colors.pi;
			break;
		case BLKFILE:
			code = data->colors.bd;
			break;
		case CHARFILE:
			code = data->colors.cd;
			break;
		case REGFILE:
			if (ls->info && ls->info->perm[2] == 'x')
				code = data->colors.ex;
			break;
	}

	if (code) {
		static char color_seq[32];
		ft_strlcpy(color_seq, "\033[", sizeof(color_seq));
		ft_strlcat(color_seq, code, sizeof(color_seq));
		ft_strlcat(color_seq, "m", sizeof(color_seq));
		return color_seq;
	}
	return "";
}
