#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	infor_t info[] = { infor_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputss(av[0]);
				_eputss(": 0: Can't open ");
				_eputss(av[1]);
				_eputchars('\n');
				_eputchars(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		infor->readfd = fd;
	}
	populate_envier_list(infor);
	read_historic(infor);
	hshs(infor, av);
	return (EXIT_SUCCESS);
}
