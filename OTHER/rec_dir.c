#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void listdir( const char *name, int indent)
{
	DIR *dir;
	struct dirent *entry;

	if ( !(dir = opendir(name)) )
		exit(EXIT_FAILURE);

	while ( (entry = readdir(dir)) != NULL ) 
	{
		if ( entry->d_type == DT_DIR ) {
			char path[1024];
			if ( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 )
				continue;
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			printf("%*s[%s]\n", indent, "", entry->d_name);
			listdir(path, indent + 2);
		} else {
			printf("%*s- %s\n", indent, "", entry->d_name);
		}
	}

	closedir(dir);
}

int main(int argc, char **argv)
{
	listdir(argv[1], 0);

	return 0;
}
