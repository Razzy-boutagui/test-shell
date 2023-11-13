#include "shell.h"

/**
 * clear_infor - initializes infor_t struct
 * @infor: struct address
 */
void clear_infor(infor_t *infor)
{
	infor->arg = NULL;
	infor->argb = NULL;
	infor->path = NULL;
	infor->argd = 0;
}

/**
 * set_infor - initializes infor_t struct
 * @infor: struct address
 * @av: argument vector
 */
void set_infor(infor_t *infor, char **av)
{
	int i = 0;

	infor->finame = av[0];
	if (infor->arg)
	{
		infor->argb = strtow(infor->arg, " \t");
		if (!infor->argb)
		{

			infor->argb = malloc(sizeof(char *) * 2);
			if (infor->argb)
			{
				infor->argb[0] = _strdup(infor->arg);
				infor->argb[1] = NULL;
			}
		}
		for (i = 0; infor->argb && infor->argb[i]; i++)
			;
		infor->argd = i;

		replaceAlias(infor);
		replaceVars(infor);
	}
}

/**
 * free_infor - frees infor_t struct fields
 * @infor: struct address
 * @all: true if freeing all fields
 */
void free_infor(infor_t *infor, int all)
{
	ffree(infor->argb);
	infor->argb = NULL;
	infor->path = NULL;
	if (all)
	{
		if (!infor->cmd_buf)
			free(infor->arg);
		if (infor->envi)
			free_list(&(infor->envi));
		if (infor->historic)
			free_list(&(infor->historic));
		if (infor->alias)
			free_list(&(infor->alias));
		ffree(infor->enviiron);
			infor->enviiron = NULL;
		bfree((void **)infor->cmd_buf);
		if (infor->readfd > 2)
			close(infor->readfd);
		_putchar(BUF_FLUSH);
	}
}
