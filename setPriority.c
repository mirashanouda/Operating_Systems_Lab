#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// int setPriority(int pid ,int pr)
// {
//     struct proc *p;
//     acquire(&ptable.lock);
//     int oldPriority;
//     for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
//         if (p->pid ==pid){
//         oldPriority = p->priority
//         p->priority = pr;
//         break;
// 	}
//     release(&ptable.lock);
//     return oldPriority;
// }

int main(int argc,char *argv[])
{
    if (argc != 3){
        printf(1,"Invalid number of arguments.\n");
        exit();
    }
    int pid= atoi(argv[1]);
    int pr= atoi(argv[2]);
    if (pr < 0 || pr > 20){
        printf(1, "Invalid priority. Choose prioirty in range 0-20\n");
        exit();
    }
    //printf(1, "PID = %d -- Pr = %d\n", pid, pr);
    int oldPr= setPriority(pid, pr);
    if (oldPr == -1)
        printf(1, "invalid PID\n");
    else printf(1, "OldPr= %d\n", oldPr);
    exit();
} 