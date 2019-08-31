#ifndef _INIREADER_H_
#define _INIREADER_H_

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

inline char file_exists(const char *path)
{
		return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES &&
			!(GetFileAttributesA(path) & FILE_ATTRIBUTE_DIRECTORY);
}

char get_path(char *inibuffer);

int read_var(const char *str);

#endif /* _INIREADER_H_ */