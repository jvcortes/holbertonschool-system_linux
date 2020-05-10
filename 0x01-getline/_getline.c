#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "_getline.h"

void *emalloc(size_t size);
void *extend(void *ptr, size_t n);
int next(const int fd, char buf[], char *line);

/**
 * _getline - retrieves a line from a file.
 *
 *  _getline will return a line from a file descriptor. If the
 * function is called various times with the same file, it will return
 * the next line until eventually, all the contents of the file are read.
 * In that case the function will return a null pointer.
 *
 * The obtained line has to be free'd when it is no longer in use.
 *
 * @fd: file descriptor.
 *
 * Return: string duplicate of the obtained line.
 */
char *_getline(const int fd)
{
	static int file, i;
	int j = i, b = 0, m = 256;
	static char buf[READ_SIZE];
	char *line;

	if (file == 0)
		file = fd;
	if (file != fd)
		return (NULL);

	line = emalloc(m * sizeof(char));
	memset(line, '\0', m);
	if (buf[i] == '\0')
	{
		switch (next(file, buf, line))
		{
			case 0:
				free(line);
				return (NULL);
			case -1:
				return (NULL);
			case 1:
				i = 0;
		}
	}

	while (buf[j] != '\n' && buf[j] != '\0')
	{
		line[b++] = buf[j++];
		if (b == m)
		{
			m += 256;
			line = extend(line, m);
		}
		if (j == READ_SIZE)
		{
			if (next(file, buf, line) < 0)
				return (NULL);
			i = j = 0;
		}
	}
	if ((READ_SIZE == 1 && buf[j] == '\n') ||
			(j == READ_SIZE - 1 && buf[j] == '\n'))
	{
		switch (next(file, buf, line))
		{
			case 0:
				return (line);
			case -1:
				return (NULL);
			case 1:
				i = 0;
		}
	}
	else if (READ_SIZE > 1 && buf[0] == '\n')
		i++;
	else
		i = j + 1;

	return (line);
}

/**
 * emalloc - allocates dynamic memory, handles null condition.
 * @size: pointer size.
 *
 * Return: pointer to the created dynamic memory space. If allocation fails,
 * the function will return a null pointer,
 */
void *emalloc(size_t size)
{
	void *ptr = malloc(size);

	if (ptr == NULL)
		free(ptr);
	return (ptr);
}

/**
 * extend - extends the amount of dynamic memory of a pointer.
 *
 * extend will create a new pointer with n size and will copy the contents
 * of the old pointer in it.
 *
 * @ptr: pointer to modify.
 * @n: size of the new pointer
 *
 * Return: pointer to the new memory area, If memory allocation fails, the
 * function will return a null pointer.
 */
void *extend(void *ptr, size_t n)
{
	void *new;

	new = emalloc(n);
	if (new == NULL)
		return (NULL);

	strncpy(new, ptr, n);
	free(ptr);

	return (new);
}

/**
 * next - wrapper for read.
 *
 * next will perform a read syscall on the buf char array with size BUF_SIZE,
 * if read fails or there's no more bytes to draw from the file. The function
 * will free the line pointer.
 *
 * @fd: file descriptor.
 * @buf: buffer to store the contents of fd.
 * @line: line used to store the current line.
 *
 * Return: return value from read, except if it is successful, in that case,
 * the function will return 1.
 */
int next(const int fd, char buf[], char *line)
{
	int s;

	memset(buf, '\0', READ_SIZE);
	s = read(fd, buf, READ_SIZE);
	if (s == 0)
		return (0);
	else if (s < 0)
	{
		free(line);
		return (-1);
	}
	return (1);
}
