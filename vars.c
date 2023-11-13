#include "shell.h"

/**
 * is_it_chain - test if current char in buffer is a chain delimeter
 * @infor: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_it_chain(infor_t *infor, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		infor->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		infor->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		infor->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @infor: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(infor_t *infor, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (infor->cmd_buf_type == CMD_AND)
	{
		if (infor->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (infor->cmd_buf_type == CMD_OR)
	{
		if (!infor->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replaceAlias - replaces an aliases in the tokenized string
 * @infor: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceAlias(infor_t *infor)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with_it(infor->alias, infor->argb[0], '=');
		if (!node)
			return (0);
		free(infor->argb[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		infor->argb[0] = p;
	}
	return (1);
}

/**
 * replaceVars - replaces vars in the tokenized string
 * @infor: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceVars(infor_t *infor)
{
	int i = 0;
	list_t *node;

	for (i = 0; infor->argb[i]; i++)
	{
		if (infor->argb[i][0] != '$' || !infor->argb[i][1])
			continue;

		if (!_strcmpare(infor->argb[i], "$?"))
		{
			replaceString(&(infor->argb[i]),
				_strdup(convert_number(infor->status, 10, 0)));
			continue;
		}
		if (!_strcmpare(infor->argb[i], "$$"))
		{
			replaceString(&(infor->argb[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with_it(infor->envi, &infor->argb[i][1], '=');
		if (node)
		{
			replaceString(&(infor->argb[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replaceString(&infor->argb[i], _strdup(""));

	}
	return (0);
}

/**
 * replaceString - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceString(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
