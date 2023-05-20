#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <dirent.h>

// DIR *opendir(const char *name)
// struct dirent *readdir(DIR *dirp);
// The  readdir() function returns a pointer to a dirent structure repre‐
// senting the next directory entry in the directory stream pointed to by
// dirp.   It returns NULL on reaching the end of the directory stream or
// if an error occurred.

// int closedir(DIR *dirp);
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getFileNum(const char* path);

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("plz key in %s path\n", argv[0]);
        return -1;
    }

    printf("文件个数：%d\n", getFileNum(argv[1]));
    return 0;
}

int getFileNum(const char* path)
{
    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("READ ERROR");
        exit(0);
    }

    struct dirent* ptr;
    int total = 0;
    
    while((ptr = readdir(dir)) != NULL)
    {
        char* dname = ptr->d_name;
        if(strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0)
            continue;
        if(ptr->d_type == DT_DIR)
        {
            char newPath[256];
            sprintf(newPath, "%s/%s", path, dname);
            total += getFileNum(newPath);
        }

        if(ptr->d_type == DT_REG)
        {
            total++;
        }
    }
    closedir(dir);
    return total;
}