#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "FSUtil.h"

void FSUTIL_ReadDir(char *path)
{
	DIR *directory;
	struct dirent *entry;

	directory = opendir(path);
	if (directory == NULL) {
		printf("[FSUtil]: ERROR: Could not open directory %s!\n", path);
		exit(1);
	}

	while (entry = readdir(directory)) {
		printf("%s\n", entry->d_name);
	}

	closedir(directory);
}
