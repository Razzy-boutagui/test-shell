#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_historic"
#define HIST_MAX	4096

extern char **enviiron;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfor - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argb: an array of strings generated from arg
 *@path: a string path for the current command
 *@argd: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@line_count_flag: if on count this line of input
 *@finame: the program filename
 *@envi: linked list local copy of enviiron
 *@enviiron: custom modified copy of enviiron from LL envi
 *@historic: the historic node
 *@alias: the alias node
 *@envi_changed: on if enviiron was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the historic line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argb;
	char *path;
	int argd;
	unsigned int line_count;
	int err_num;
	int line_count_flag;
	char *finame;
	list_t *envi;
	list_t *historic;
	list_t *alias;
	char **enviiron;
	int envi_changed;
	int status;
	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} infor_t;

#define infor_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(infor_t *);
} builtin_table;


/* toem_shloop.c */
int hshs(infor_t *, char **);
int search_bultin(infor_t *);
void search_cmd(infor_t *);
void branch_cmd(infor_t *);

/* toem_parser.c */
int is_it_cmd(infor_t *, char *);
char *repeated_chars(char *, int, int);
char *search_path(infor_t *, char *, char *);

/* loophshs.c */
int loophshs(char **);

/* toem_errors.c */
void _eputss(char *);
int _eputchars(char);
int _write_putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strleng(char *);
int _strcmpare(char *, char *);
char *starts_with_it(const char *, const char *);
char *_strgcat(char *, char *);

/* toem_string1.c */
char *_strcopy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_a_toi.c */
int interactive(infor_t *);
int is_it_delimet(char, char *);
int _is_it_alphab(int);
int _a_toi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void print_error(infor_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int _myexit(infor_t *);
int _mycd(infor_t *);
int _myhelp(infor_t *);

/* toem_builtin1.c */
int _myhistoric(infor_t *);
int _myalias(infor_t *);

/*toem_getline.c */
ssize_t get_input(infor_t *);
int _getline(infor_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfor.c */
void clear_infor(infor_t *);
void set_infor(infor_t *, char **);
void free_infor(infor_t *, int);

/* toem_enviiron.c */
char *_getenvi(infor_t *, const char *);
int _myenvi(infor_t *);
int _mysetenvi(infor_t *);
int _myunsetenvi(infor_t *);
int populate_envier_list(infor_t *);

/* toem_getenvi.c */
char **get_enviiron(infor_t *);
int _unsetenvi(infor_t *, char *);
int _setenvi(infor_t *, char *, char *);

/* toem_historic.c */
char *get_historic_file(infor_t *infor);
int write_historic(infor_t *infor);
int read_historic(infor_t *infor);
int build_historic_list(infor_t *infor, char *buf, int linecount);
int renumber_historic(infor_t *infor);

/* toem_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_dex(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_length(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with_it(list_t *, char *, char);
ssize_t get_node_dex(list_t *, list_t *);

/* toem_vars.c */
int is_it_chain(infor_t *, char *, size_t *);
void check_chain(infor_t *, char *, size_t *, size_t, size_t);
int replaceAlias(infor_t *);
int replaceVars(infor_t *);
int replaceString(char **, char *);

#endif