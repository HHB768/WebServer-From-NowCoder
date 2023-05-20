#include <stdio.h>

// #define DEBUG

int main()
{
    int a = 0;
    printf("Hello GCC!\n");
#ifdef DEBUG
    printf("I am a coder now!\n");
#endif
}