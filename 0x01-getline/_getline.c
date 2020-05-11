#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "_getline.h"

void *emalloc(size_t size);
void *extend(void *ptr, size_t n);
void remove_file(File **files, size_t *size, const int fd);
int next(const int fd, char buf[], char *line);
int search_or_add(File **files, size_t *size, const int fd);

/**
 * _getline - retrieves a line from a file.
 *  _getline will return a line from a file descriptor. If the
 * function is called various times with the same file, it will return
 * the next line until eventually, all the contents of the file are read.
 * In that case the function will return a null pointer.
 *
 * The obtained line has to be free'd when it is no longer in use.
 *
 * @fd: file descriptor.
 *
 * Return: obtained line.
 */
char *_getline(const int fd)
{
	static File *files;
	static size_t size;
	File *file;
	int i, j, b = 0, m = 256;
	char *line;

	if (fd == -1)
	{
		if (size)
		{
			free(files);
			files = NULL;
			size = 0;
		}
		return (NULL);
	}

	i = search_or_add(&files, &size, fd);
	if (i == -1)
		return (NULL);
	file = &files[i];

	j = file->index;
	line = emalloc(m * sizeof(char));
	if (line == NULL)
		return (NULL);
	memset(line, '\0', m);
	if (file->buf[file->index] == '\0')
	{
		switch (next(file->fd, file->buf, line))
		{
			case 0:
				remove_file(&files, &size, fd);
				free(line);
				return (NULL);
			case -1:
				return (NULL);
			case 1:
				file->index = 0;
		}
	}

	while (file->buf[j] != '\n' && file->buf[j] != '\0')
	{
		line[b++] = file->buf[j++];
		if (b == m)
		{
			m += 256;
			line = extend(line, m);
			if (line == NULL)
				return (NULL);
		}
		if (j == READ_SIZE)
		{
			if (next(file->fd, file->buf, line) < 0)
				return (NULL);
			file->index = j = 0;
		}
	}
	if ((READ_SIZE == 1 && file->buf[j] == '\n') ||
			(j == READ_SIZE - 1 && file->buf[j] == '\n'))
	{
		switch (next(file->fd, file->buf, line))
		{
			case 0:
				file->status = 1;
				return (line);
			case -1:
				return (NULL);
			case 1:
				file->index = 0;
		}
	}
	else if (READ_SIZE > 1 && file->index == 0 && file->buf[0] == '\n')
		file->index++;
	else
		file->index = j + 1;

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

/**
 * search_or_add - looks for the index of a File struct.
 *
 * search_or_add will look for the File struct in the files array, if it is
 * associated with fd.  If no File is found, it will be created.
 *
 * @files: array of files.
 * @size: size of the files array.
 * @fd: file descriptor to search.
 *
 * Return: size of the resulting array. If memory allocation fails, the
 * function will return -1.
 */
int search_or_add(File **files, size_t *size, const int fd)
{
	File *of = *files;
	int i = 0;

	if (*files != NULL)
	{
		for (i = 0; i < (int) *size; i++)
			if ((*files)[i].fd == fd)
				return (i);

		*files = emalloc((*size + 1) * sizeof(File));
		if (*files == NULL)
			return (-1);

		for (i = 0; i < (int) *size; i++)
			(*files)[i] = of[i];
		(*files)[i].fd = fd;
		(*files)[i].status = 0;
		(*files)[i].index = 0;
		memset((*files)[i].buf, '\0', READ_SIZE);
		*size += 1;
		free(of);
	}
	else
	{
		*files = emalloc((*size + 1) * sizeof(File));
		if (*files == NULL)
			return (-1);

		(*files)[i].fd = fd;
		(*files)[i].status = 0;
		(*files)[i].index = 0;
		memset((*files)[i].buf, '\0', READ_SIZE);
		*size += 1;
	}

	return (i);
}

/**
 * remove_file - removes a File struct from a array of Files.
 *
 * If no file assocated with the file descriptor fd. No operations
 * will be performed.
 *
 * @files: array of files.
 * @size: size of the files array.
 * @fd: file descriptor, used to search the file to delete.
 *
 * Return: nothing.
 */
void remove_file(File **files, size_t *size, const int fd)
{
	File *of = *files;
	size_t osz;
	int i = 0, j = 0, del = -1;

	if (*files != NULL)
	{
		for (i = 0; i < (int) *size; i++)
			if ((*files)[i].fd == fd)
			{
				del = i;
				break;
			}
		if (del == -1)
			return;

		if (*size - 1 == 0)
		{
			files = NULL;
			return;
		}
		osz = *size;
		*files = emalloc((*size - 1) * sizeof(File));
		if (*files == NULL)
			return;

		for (i = 0; i < (int) osz; i++)
			if (of[i].fd != of[del].fd)
				(*files)[j++] = of[i];
		*size -= 1;
		free(of);
	}
}
