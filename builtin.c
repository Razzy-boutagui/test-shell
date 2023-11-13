#include "shell.h"

/**
 * _myexit - exits the shell
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if infor.argb[0] != "exit"
 */
int _myexit(infor_t *infor)
{
	int exitcheck;

	if (infor->argb[1])  /* If there is an exit arguement */
	{
		exitcheck = _erratoi(infor->argb[1]);
		if (exitcheck == -1)
		{
			infor->status = 2;
			print_error(infor, "Illegal number: ");
			_eputss(infor->argb[1]);
			_eputchars('\n');
			return (1);
		}
		infor->err_num = _erratoi(infor->argb[1]);
		return (-2);
	}
	infor->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _mycd(infor_t *infor)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!infor->argb[1])
	{
		dir = _getenvi(infor, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenvi(infor, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmpare(infor->argb[1], "-") == 0)
	{
		if (!_getenvi(infor, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenvi(infor, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenvi(infor, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(infor->argb[1]);
	if (chdir_ret == -1)
	{
		print_error(infor, "can't cd to ");
		_eputss(infor->argb[1]), _eputchars('\n');
	}
	else
	{
		_setenvi(infor, "OLDPWD", _getenvi(infor, "PWD="));
		_setenvi(infor, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @infor: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myhelp(infor_t *infor)
{
	char **arg_array;

	arg_array = infor->argb;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
