#include <stdlib.h>

/**
 * _strlen - returns the length of a string
 * @s: provided string
 *
 * Return: length of the string.
 */
int _strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;

	return (i);
}

/**
 * _strncat - concatenates n bytes from a string to a destination.
 * @dest: destination string
 * @src: string to concatenate at the end of the destination
 * @n: amount of bytes to take from source string
 * Return: str, pointer to the resulting string
 */
char *_strncat(char *dest, char *src, int n)
{
	int i, j;
	char *str;

	str = dest;

	for (i = 0; dest[i] != '\0'; i++)
		;

	for (j = 0; src[j] != '\0' && j < n; j++, i++)
		str[i] = src[j];

	str[i + 1] = '\0';
	return (str);
}

/**
 * _strncpy - copies a string
 * @dest: string to hold the copy
 * @src: string to be copied
 * @n: amount of bytes to be copied from the source string
 * Return: dest, the copied string
 */
char *_strncpy(char *dest, const char *src, int n)
{
	int i, j;

	if (*src == '\0')
		*dest = '\0';
	for (i = 0; src[i] != '\0'; i++)
		;

	for (j = 0; src[j] != '\0' && j < n; j++)
		dest[j] = src[j];

	if (i < n)
		for (; j < n; j++)
			dest[j] = '\0';
	return (dest);
}

/**
 * _strdup - duplicates a string.
 * @s: pointer to the string.
 *
 * Return: pointer to the newly allocated string duplicate.
 */
char *_strdup(const char *s)
{
	size_t len = _strlen(s) + 1;
	char *new = malloc(len);
	if (new == NULL)
	{
		free(new);
		return (NULL);
	}

	return _strncpy(new, s, len);
}
