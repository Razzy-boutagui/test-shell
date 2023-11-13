#include "shell.h"

/**
 * hshs - main shell loop
 * @infor: the parameter & return infor struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hshs(infor_t *infor, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_infor(infor);
		if (interactive(infor))
			_puts("$ ");
		_eputchars(BUF_FLUSH);
		r = get_input(infor);
		if (r != -1)
		{
			set_infor(infor, av);
			builtin_ret = search_bultin(infor);
			if (builtin_ret == -1)
				search_cmd(infor);
		}
		else if (interactive(infor))
			_putchar('\n');
		free_infor(infor, 0);
	}
	write_historic(infor);
	free_infor(infor, 1);
	if (!interactive(infor) && infor->status)
		exit(infor->status);
	if (builtin_ret == -2)
	{
		if (infor->err_num == -1)
			exit(infor->status);
		exit(infor->err_num);
	}
	return (builtin_ret);
}

/**
 * search_bultin - finds a builtin command
 * @infor: the parameter & return infor struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int search_bultin(infor_t *infor)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"envi", _myenvi},
		{"help", _myhelp},
		{"historic", _myhistoric},
		{"setenvi", _mysetenvi},
		{"unsetenvi", _myunsetenvi},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmpare(infor->argb[0], builtintbl[i].type) == 0)
		{
			infor->line_count++;
			built_in_ret = builtintbl[i].func(infor);
			break;
		}
	return (built_in_ret);
}

/**
 * search_cmd - finds a command in PATH
 * @infor: the parameter & return infor struct
 *
 * Return: void
 */
void search_cmd(infor_t *infor)
{
	char *path = NULL;
	int i, k;

	infor->path = infor->argb[0];
	if (infor->line_count_flag == 1)
	{
		infor->line_count++;
		infor->line_count_flag = 0;
	}
	for (i = 0, k = 0; infor->arg[i]; i++)
		if (!is_it_delimet(infor->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = search_path(infor, _getenvi(infor, "PATH="), infor->argb[0]);
	if (path)
	{
		infor->path = path;
		branch_cmd(infor);
	}
	else
	{
		if ((interactive(infor) || _getenvi(infor, "PATH=")
			|| infor->argb[0][0] == '/') && is_it_cmd(infor, infor->argb[0]))
			branch_cmd(infor);
		else if (*(infor->arg) != '\n')
		{
			infor->status = 127;
			print_error(infor, "not found\n");
		}
	}
}

/**
 * branch_cmd - forks a an exec thread to run cmd
 * @infor: the parameter & return infor struct
 *
 * Return: void
 */
void branch_cmd(infor_t *infor)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(infor->path, infor->argb, get_enviiron(infor)) == -1)
		{
			free_infor(infor, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(infor->status));
		if (WIFEXITED(infor->status))
		{
			infor->status = WEXITSTATUS(infor->status);
			if (infor->status == 126)
				print_error(infor, "Permission denied\n");
		}
	}
}
