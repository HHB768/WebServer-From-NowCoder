#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("hello\n");  // \n 也会提示它fflush cache
    printf("world");   // exit会检查，_exit不会，遂寄


    // exit(0);
    _exit(0);

    return 0;
}