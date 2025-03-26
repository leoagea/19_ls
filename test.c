#include <stdio.h>    // printf, perror
#include <stdlib.h>   // exit, EXIT_FAILURE
#include <dirent.h>   // DIR, struct dirent, opendir, readdir, closedir
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int print_file_info(const char *filepath) {
    struct stat sb;
    if (stat(filepath, &sb) == -1) {
        perror("stat");
        return -1;
    }

    // Convert the file's last modification time to local time
    struct tm *mod_time = localtime(&sb.st_mtime);
    if (!mod_time) {
        perror("localtime");
        return -1;
    }

    // Create a buffer and format the time into a readable string
    char time_str[64];
    if (strftime(time_str, sizeof(time_str), "%c", mod_time) == 0) {
        // If formatting fails or the buffer is too small, strftime returns 0
        fprintf(stderr, "Error formatting time\n");
        return -1;
    }

    // Print some file information
    printf("File: %s\n", filepath);
    printf("Size: %lld bytes\n", (long long)sb.st_size);
    printf("Last modification: %s\n", time_str);

    return 0;
}

int main(void)
{

    const char *path = "srcs/init.c";  // Directory you want to open; current dir as example
	print_file_info(path);
    // DIR *dir = opendir(path);
    // if (dir == NULL)
    // {
    //     perror("opendir");    // Print the error (e.g., "No such file or directory")
    //     char *err = strerror(errno);
	// 	printf("err msg: %s\n", err);
	// 	return EXIT_FAILURE;
    // }

    // struct dirent *entry;
    // while ((entry = readdir(dir)) != NULL)
    // {
    //     // d_name is the name of the file/directory within 'path'
    //     printf("%s			type:%u\n", entry->d_name, entry->d_type);
    // }

    // // Close the directory stream to free resources
    // if (closedir(dir) == -1)
    // {
    //     perror("closedir");
    //     return EXIT_FAILURE;
    // }

    // return EXIT_SUCCESS;
}
