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
 * @k: current amount of used space inside the buf array.
 * @buf: buffer to store the contents generated from read.
 */
typedef struct file_t
{
	int fd;
	int index;
	int status;
	int k;
	char buf[READ_SIZE];
} File;

char *_getline(const int fd);

#endif /* ifndef GETLINE */
