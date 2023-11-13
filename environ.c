#include "shell.h"

/**
 * _myenvi - prints the current enviironment
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _myenvi(infor_t *infor)
{
	print_list_str(infor->envi);
	return (0);
}

/**
 * _getenvi - gets the value of an enviiron variable
 * @infor: Structure containing potential arguments. Used to maintain
 * @name: envi var name
 *
 * Return: the value
 */
char *_getenvi(infor_t *infor, const char *name)
{
	list_t *node = infor->envi;
	char *p;

	while (node)
	{
		p = starts_with_it(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenvi - Initialize a new enviironment variable,
 *             or modify an existing one
 * @infor: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _mysetenvi(infor_t *infor)
{
	if (infor->argd != 3)
	{
		_eputss("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenvi(infor, infor->argb[1], infor->argb[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenvi - Remove an enviironment variable
 * @infor: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myunsetenvi(infor_t *infor)
{
	int i;

	if (infor->argd == 1)
	{
		_eputss("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= infor->argd; i++)
		_unsetenvi(infor, infor->argb[i]);

	return (0);
}

/**
 * populate_envier_list - populates envi linked list
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_envier_list(infor_t *infor)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; enviiron[i]; i++)
		add_node_end(&node, enviiron[i], 0);
	infor->envi = node;
	return (0);
}
