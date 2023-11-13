#include "shell.h"

/**
 * get_enviiron - returns the string array copy of our enviiron
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_enviiron(infor_t *infor)
{
	if (!infor->enviiron || infor->envi_changed)
	{
		infor->enviiron = list_to_strings(infor->envi);
		infor->envi_changed = 0;
	}

	return (infor->enviiron);
}

/**
 * _unsetenvi - Remove an enviironment variable
 * @infor: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string envi var property
 */
int _unsetenvi(infor_t *infor, char *var)
{
	list_t *node = infor->envi;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with_it(node->str, var);
		if (p && *p == '=')
		{
			infor->envi_changed = delete_node_at_dex(&(infor->envi), i);
			i = 0;
			node = infor->envi;
			continue;
		}
		node = node->next;
		i++;
	}
	return (infor->envi_changed);
}

/**
 * _setenvi - Initialize a new enviironment variable,
 *             or modify an existing one
 * @infor: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string envi var property
 * @value: the string envi var value
 *  Return: Always 0
 */
int _setenvi(infor_t *infor, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strleng(var) + _strleng(value) + 2);
	if (!buf)
		return (1);
	_strcopy(buf, var);
	_strgcat(buf, "=");
	_strgcat(buf, value);
	node = infor->envi;
	while (node)
	{
		p = starts_with_it(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			infor->envi_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(infor->envi), buf, 0);
	free(buf);
	infor->envi_changed = 1;
	return (0);
}
