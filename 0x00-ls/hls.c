#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "strutil.c"


typedef struct file_s
{
	char path[4096];
	char name[256];
	char type;
	char perm[9];
	char *time;
	long size;
	char *user;
	char *group;
} File;

File **getlist(char *path);
File **createlist(size_t size);
int getfilecount(char *path);


File
**createlist(size_t size)
{
	int i;
	File **files;

	if (size == 0)
		return (NULL);

	files = malloc(size * sizeof(File *));
	if (files == NULL)
	{
		free(files);
		return (NULL);
	}

	for (i = 0; (size_t) i < size; i++)
	{
		files[i] = malloc(sizeof(File));
		if (files[i] == NULL)
		{
			while (i >= 0)
				free(files[i--]);
			return (NULL);
		}
	}

	files[size - 1] = NULL;

	return (files);
}

File 
**getlist(char *path)
{
	DIR *dir;
	struct dirent *read;
	struct stat filestat;
	ssize_t size = getfilecount(path);
	File **files;
	unsigned int i = 0;
	char *c;

	files = createlist(size + 1);

	dir = opendir(path);
	if (dir == NULL)
	{
		fprintf(stderr,
			"hls: cannot access '%s': No such file or directory\n",
			path);
		return (NULL);
	}

	while ((read = readdir(dir)) != NULL)
	{
		memcpy(files[i]->name, read->d_name, strlen(read->d_name));
		files[i]->name[strlen(read->d_name)] = '\0';
		memcpy(files[i]->path, path, strlen(path));
		files[i]->path[strlen(path)] = '\0';
		strncat(files[i]->path, files[i]->name, strlen(files[i]->name));
		files[i]->path[strlen(files[i]->path)] = '\0';

		if ((lstat(files[i]->path, &filestat)) == -1)
		{
			exit (2);
		}

		switch (filestat.st_mode & S_IFMT)
		{
			case S_IFREG:  files[i]->type = '-'; break;
			case S_IFDIR:  files[i]->type = 'd'; break;
			case S_IFCHR:  files[i]->type = 'c'; break;
			case S_IFBLK:  files[i]->type = 'b'; break;
			case S_IFIFO:  files[i]->type = 'p'; break;
			case S_IFLNK:  files[i]->type = 'l'; break;
			case S_IFSOCK: files[i]->type = 's'; break;
		}

		files[i]->perm[0] = filestat.st_mode & S_IRUSR ? 'r' : '-';
		files[i]->perm[1] = filestat.st_mode & S_IWUSR ? 'w' : '-';
		files[i]->perm[2] = filestat.st_mode & S_IXUSR ? 'x' : '-';
		files[i]->perm[3] = filestat.st_mode & S_IRGRP ? 'r' : '-';
		files[i]->perm[4] = filestat.st_mode & S_IWGRP ? 'w' : '-';
		files[i]->perm[5] = filestat.st_mode & S_IXGRP ? 'x' : '-';
		files[i]->perm[6] = filestat.st_mode & S_IROTH ? 'r' : '-';
		files[i]->perm[7] = filestat.st_mode & S_IROTH ? 'w' : '-';
		files[i]->perm[8] = filestat.st_mode & S_IROTH ? 'x' : '-';

		files[i]->time = ctime(&(filestat.st_mtime));
		files[i]->time[strlen(files[i]->time) - 9] = '\0';
		files[i]->size = filestat.st_size;

		c = getpwuid(filestat.st_uid)->pw_name;
		if (strlen(c))
		{
			files[i]->user = malloc(strlen(c) * sizeof(char));
			files[i]->user = strncpy(files[i]->user, c, strlen(c));
		}
		
		c = getgrgid(filestat.st_uid)->gr_name;
		if (strlen(c))
		{
			files[i]->group = malloc(strlen(c) * sizeof(char));
			files[i]->group = strncpy(files[i]->group, c, strlen(c));
		}

		i++;
	}

	return (files);
}

int
getfilecount(char *path)
{
	DIR *dir;
	struct dirent *read;
	int count = 0;

	dir = opendir(path);
	if (dir == NULL)
	{
		fprintf(stderr,
			"hls: cannot access '%s': No such file or directory\n",
			path);
		return (-1);
	}

	while ((read = readdir(dir)) != NULL)
		count++;

	closedir(dir);

	return (count);
}

int main(int argc, char *argv[])
{
	int i = 0;
	File **files;

	if (argc == 2)
	{
		files = getlist(argv[1]);
		while (files[i] != NULL)
		{
			printf("%s", files[i]->name);
			if (files[i + 1])
				printf("  ");
			else
				printf("\n");
			i++;
		}
	}

	return (0);
}
