#include <stdio.h>
#include <unistd.h>

int main()
{
    while(1)
    {
        printf("my pid: [%d]\n", getpid());
    }
}