#include "inireader.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

char pathbuffer[256];

char get_path(char *inibuffer)
{
		GetCurrentDirectory(sizeof(pathbuffer), pathbuffer);
		strncat(pathbuffer, "\\", 2);
		strncat(pathbuffer, inibuffer, strlen(inibuffer));

		return file_exists(pathbuffer);
}

int read_var(const char *str)
{
		char linebuffer[256];
		FILE *fd = fopen(pathbuffer, "r");

		if (fd) {
			char option[32];
			char valbuf[32];
			int value;

			while (fgets(linebuffer, sizeof(linebuffer), fd) != NULL) {
				if (linebuffer[0] == '#' || linebuffer[0] == '[' || linebuffer[0] == '\n')
					continue;

					sscanf(linebuffer, "%s = %s", option, &valbuf);
					value = strtol(valbuf, NULL, 0);

					if (strcmp(option, str) == 0) {
						fclose(fd);
						return value;
				}
			}
		}
		return -1;
}