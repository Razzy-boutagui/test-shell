#include "shell.h"

/**
 * _myhistoric - displays the historic list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @infor: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myhistoric(infor_t *infor)
{
	print_list(infor->historic);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @infor: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(infor_t *infor, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_dex(&(infor->alias),
		get_node_dex(infor->alias, node_starts_with_it(infor->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @infor: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(infor_t *infor, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(infor, str));

	unset_alias(infor, str);
	return (add_node_end(&(infor->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias(infor_t *infor)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (infor->argd == 1)
	{
		node = infor->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; infor->argb[i]; i++)
	{
		p = _strchr(infor->argb[i], '=');
		if (p)
			set_alias(infor, infor->argb[i]);
		else
			print_alias(node_starts_with_it(infor->alias, infor->argb[i], '='));
	}

	return (0);
}
