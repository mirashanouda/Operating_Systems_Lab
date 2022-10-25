#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{

    printf(1,"The number of system calls till now is %d\n", printcalls());
    exit();
}