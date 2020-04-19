#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "def.h"


DIR
*open_directory(char **path)
{
	int trailing_slash = 0;
	int path_len = 0;
	char *tmp;
	DIR *dir;

	if (path == NULL || *path == NULL)
		return (NULL);

	path_len = strlen(*path);

	if (path[0][path_len - 1] != '/')
		trailing_slash = 1;
	tmp = malloc(path_len + trailing_slash + 1);
	if (tmp == NULL)
	{
		free(tmp);
		return (NULL);
	}

	strncpy(tmp, *path, path_len + trailing_slash + 1);
	if (trailing_slash)
		tmp[path_len] = '/';
	*path = tmp;

	dir = opendir(*path);
	if (dir == NULL)
	{
		fprintf(stderr,
			"hls: cannot access '%s': No such file or directory\n",
			*path);
		free(*path);
		exit(1);
	}

	return (dir);
}

File
**create_list(size_t size)
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

	printf("%ld\n", size);
	for (i = 0; (size_t) i < size - 1; i++)
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
**get_list(char *path)
{
	DIR *dir;
	struct dirent *read;
	struct stat filestat;
	ssize_t size = file_count(path);
	File **files;
	unsigned int i = 0;
	char *c;

	files = create_list(size + 1);
	dir = open_directory(&path);

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
			exit(2);
		}

		switch (filestat.st_mode & S_IFMT)
		{
			case S_IFREG:
				files[i]->type = '-';
				break;
			case S_IFDIR:
				files[i]->type = 'd';
				break;
			case S_IFCHR:
				files[i]->type = 'c';
				break;
			case S_IFBLK:
				files[i]->type = 'b';
				break;
			case S_IFIFO:
				files[i]->type = 'p';
				break;
			case S_IFLNK:
				files[i]->type = 'l';
				break;
			case S_IFSOCK:
				files[i]->type = 's';
				break;
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

	free(read);
	free(dir);
	free(path);
	return (files);
}

int
file_count(char *path)
{
	DIR *dir;
	struct dirent *read;
	int count = 0;

	dir = open_directory(&path);
	while ((read = readdir(dir)) != NULL)
		count++;

	closedir(dir);

	free(path);
	return (count);
}

void
cleanup(File **file_list)
{
	int i = 0;

	while (file_list[i] != NULL)
	{
		free(file_list[i]->user);
		free(file_list[i]->group);
		free(file_list[i++]);
	}

	free(file_list);
}

int main(int argc, char *argv[])
{
	int i = 0;
	File **files;

	if (argc == 2)
	{
		files = get_list(argv[1]);
		while (files[i] != NULL)
		{
			printf("%s", files[i]->name);
			if (files[i + 1])
				printf("  ");
			else
				printf("\n");
			i++;
		}
		cleanup(files);
	}

	return (0);
}
