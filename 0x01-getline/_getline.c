#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "_getline.h"


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
	static char buf[READ_SIZE];
	int j, b = 0;
	char line[READ_SIZE];

	if (file == 0)
	{
		file = fd;
		memset(buf, '\0', READ_SIZE);
	}
	if (file != fd)
		return (NULL);
	memset(line, '\0', READ_SIZE);
	if (!buf[i])
	{
		if (read(file, buf, READ_SIZE) == 0)
			return (NULL);

	}
	j = i;

	while (buf[i])
	{
		if (buf[j] != '\n')
			line[b++] = buf[j++];
		else
			break;
		if (j == READ_SIZE)
		{
			i = 0;
			j = 0;
			memset(buf, '\0', READ_SIZE);
			if (read(file, buf, READ_SIZE) == 0)
				return (NULL);
		}
	}
	i = j + 1;

	return (strdup(line));
}
