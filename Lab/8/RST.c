#include <stdio.h>
#define INT_MAX 2147483647
struct process{
    int pid;
    int bt;
    int wt,tt;
    int at;
};
int pre_process, pre_time;

int rem_shortest_job(int num, struct process *p, int curr_time){
    int min_burst = INT_MAX, min_index = INT_MAX;
    if (curr_time > 0){
        p[pre_process].bt = p[pre_process].bt -(curr_time-pre_time);
    }
    for (int i = 0; i <= num; i++) {
        if(p[i].at <= curr_time) {
            if (p[i].bt > 0 && p[i].bt < min_burst) {
                min_burst = p[i].bt;
                min_index = p[i].pid;
            }
        }
    }
    pre_process = min_index;
    pre_time = curr_time;
}
int main(){
    int process_num;
    int burst_time_sum = 0;
    scanf("%d", &process_num);
    struct process p[process_num];
    for (int i = 0; i<process_num; i++){
        int service_time, arrival_time;
        scanf("%d", &arrival_time);
        scanf("%d", &service_time);
        p[i].bt = service_time;
        burst_time_sum += service_time;
        p[i].at = arrival_time;
        p[i].pid = i;
    }
    for (int j = 0; j < burst_time_sum; j++) {
        if (p[j].at == j) {
            rem_shortest_job(process_num, p, j);
            printf("%d  ", pre_process);
            printf("%d  \n", pre_time);
        }
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
    printf("Average Waiting Time Is : %.1f\n",(int ) avr_waiting/process_num);
    printf("Average Execution Time Is : %.1f\n", (int ) avr_execution/process_num);
}










