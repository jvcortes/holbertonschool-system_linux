#include <stdlib.h>
#include "def.h"

/**
 * strstrp - remove characters from a string.
 * @s: pointer to the string.
 * @c: characters to remove.
 *
 * Return: pointer to the resulting string, if memory allocation fails, the
 * function will return a null pointer.
 */
char *strstrp(char *s, char *c)
{
	int i, j, new_len = 0;
	char *new;

	for (i = 0; s[i] != '\0'; i++)
		if (stridx(c, s[i]) == -1)
			new_len++;

	if (new_len)
	{
		new = malloc(new_len * sizeof(char) + sizeof(char));
		if (new == NULL)
		{
			free(new);
			return (NULL);
		}
	}
	else
	{
		return (NULL);
	}

	for (i = 0, j = 0; s[i] != '\0'; i++)
		if (stridx(c, s[i]) == -1)
			new[j++] = s[i];
	new[j] = '\0';

	return (new);
}


/**
 * stridx - retrieves the index of a first occurrence of a character inside a
 * string.
 *
 * @s: pointer to the string.
 * @c: character to search.
 *
 * Return: index of the first occurrence of the character, if no character is
 * found, the function will return -1.
 */
int stridx(char *s, char c)
{
	int i = 0;

	if (s == NULL)
		return (-1);

	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}

	return (-1);
}
