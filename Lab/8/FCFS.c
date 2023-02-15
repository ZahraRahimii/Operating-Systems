#include <stdio.h>
struct process{
    int pid;
    int bt;
    int wt,tt;
    int at;
};

int main() {
    int process_num;
    scanf("%d", &process_num);
    struct process p[process_num];

    for (int i = 0; i<process_num; i++){
        int service_time;
        scanf("%d", &service_time);
        p[i].bt = service_time;
        p[i].pid = i;
    }
    p[0].wt = 0;
    p[0].tt = p[0].bt;
    for (int j = 1; j < process_num ; j++) {
        p[j].wt = p[j-1].tt; //waiting time
        p[j].tt = p[j].wt + p[j].bt; //process execution time
    }
    int avr_waiting = 0, avr_execution = 0;
    for (int k = 0; k < process_num; ++k) {
        avr_waiting += p[k].wt;
        avr_execution += p[k].tt;
    }
    printf("Average Waiting Time Is : %.1f\n",(float ) avr_waiting/process_num);
    printf("Average Execution Time Is : %.1f\n", (float ) avr_execution/process_num);
    return 0;
}
