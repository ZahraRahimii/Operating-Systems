#include <stdio.h>
#define INT_MAX 2147483647

struct process{
    int pid;
    int bt;
    int wt,tt;
    int prio;
};
int find_high_prio(int num, int *tmp_burst_time, int *tmp_priority){
    int min_prio = INT_MAX, min_index;
    for (int i = 0; i < num; i++) {
        if(tmp_priority[i] < min_prio){
            min_prio = tmp_priority[i];
            min_index = i;
        }
    }
    tmp_priority[min_index] = INT_MAX;
    return tmp_burst_time[min_index];
}


int main(){
    int process_num;
    scanf("%d", &process_num);
    struct process p[process_num];
    int tmp_burst_time[process_num];
    int tmp_priority[process_num];

    for (int i = 0; i<process_num; i++){
        int service_time, prio;
        scanf("%d", &service_time);
        scanf("%d", &prio);
        tmp_burst_time[i] = service_time;
        tmp_priority[i] = prio;
    }
    for (int j = 0; j < process_num; j++) {
        p[j].bt = find_high_prio(process_num, tmp_burst_time, tmp_priority);
    }//Now The problem is like FCFS
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


