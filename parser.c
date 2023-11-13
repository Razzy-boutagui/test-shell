#include "shell.h"

/**
 * is_it_cmd - determines if a file is an executable command
 * @infor: the infor struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_it_cmd(infor_t *infor, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * repeated_chars - repeats characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *repeated_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * search_path - finds this cmd in the PATH string
 * @infor: the infor struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *search_path(infor_t *infor, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strleng(cmd) > 2) && starts_with_it(cmd, "./"))
	{
		if (is_it_cmd(infor, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = repeated_chars(pathstr, curr_pos, i);
			if (!*path)
				_strgcat(path, cmd);
			else
			{
				_strgcat(path, "/");
				_strgcat(path, cmd);
			}
			if (is_it_cmd(infor, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
