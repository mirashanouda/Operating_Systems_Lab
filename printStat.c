#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf(1,"Invalid number of arguments.\n");
        exit();
    }
    printStat(atoi(argv[1]));
    exit();
}