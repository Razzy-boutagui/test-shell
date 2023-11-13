#include "shell.h"

/**
 * get_historic_file - gets the historic file
 * @infor: parameter struct
 *
 * Return: allocated string containg historic file
 */

char *get_historic_file(infor_t *infor)
{
	char *buf, *dir;

	dir = _getenvi(infor, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strleng(dir) + _strleng(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcopy(buf, dir);
	_strgcat(buf, "/");
	_strgcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_historic - creates a file, or appends to an existing file
 * @infor: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_historic(infor_t *infor)
{
	ssize_t fd;
	char *filename = get_historic_file(infor);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = infor->historic; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_write_putfd('\n', fd);
	}
	_write_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_historic - reads historic from file
 * @infor: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_historic(infor_t *infor)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_historic_file(infor);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_historic_list(infor, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_historic_list(infor, buf + last, linecount++);
	free(buf);
	infor->histcount = linecount;
	while (infor->histcount-- >= HIST_MAX)
		delete_node_at_dex(&(infor->historic), 0);
	renumber_historic(infor);
	return (infor->histcount);
}

/**
 * build_historic_list - adds entry to a historic linked list
 * @infor: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the historic linecount, histcount
 *
 * Return: Always 0
 */
int build_historic_list(infor_t *infor, char *buf, int linecount)
{
	list_t *node = NULL;

	if (infor->historic)
		node = infor->historic;
	add_node_end(&node, buf, linecount);

	if (!infor->historic)
		infor->historic = node;
	return (0);
}

/**
 * renumber_historic - renumbers the historic linked list after changes
 * @infor: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_historic(infor_t *infor)
{
	list_t *node = infor->historic;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (infor->histcount = i);
}
