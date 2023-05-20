#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    union {
        short value;
        char bytes[sizeof(short)];
    } test;
    test.value = 0x0102;
    if(test.bytes[0] == 0x01)
    {
        printf("Big-endian order\n");
    }
    else if(test.bytes[0] == 0x02)
    {
        printf("Little-endian order\n");
    }
    else
    {
        printf("?\n");
    }

    unsigned short a = 0x0102;
    printf("a : %x\n", a);
    unsigned short b = htons(a);
    printf("b : %x\n", b);

    char buf[4] = {192, 168, 1, 100};
    int num = *(int *)buf;
    int sum = htonl(num);
    unsigned char * p = (char *)&sum;

    printf("%d %d %d %d\n", *p, *(p+1), *(p+2), *(p+3));

    unsigned char buf1[4] = {100, 1, 168, 192};
    int num1 = *(int *)buf1;
    int sum1 = ntohl(num1);
    unsigned char * p1 = (unsigned char *)&sum1;

    printf("%d %d %d %d\n", *p1, *(p1+1), *(p1+2), *(p1+3));



    return 0;
}