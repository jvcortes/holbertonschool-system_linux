#ifndef GETLINE
#define GETLINE

#define READ_SIZE 1024

/**
 * struct file_t - defines a file.
 *
 * @fd: file descriptor.
 * @index: index, used to iterate though the file's contents
 * using the read syscall.
 * @status: indicates the read state of the file:
 *  - 0: the file is still pending to be read completely.
 *  - 1: the file has been read completely.
 * @buf: buffer to store the contents generated from read.
 */
typedef struct file_t
{
	int fd;
	int index;
	int status;
	char buf[READ_SIZE];
} File;

char *_getline(const int fd);

#endif /* ifndef GETLINE */
