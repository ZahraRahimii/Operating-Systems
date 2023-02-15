#include <stdio.h>
#define INT_MAX 2147483647
struct process{
    int pid;
    int bt;
    int wt,tt;
};
int sort_shortest_job(int num, int burst_time[num]){
    int min_burst = INT_MAX, min_index;
    for (int i = 0; i < num; i++) {
        if(burst_time[i] < min_burst){
            min_burst = burst_time[i];
            min_index = i;
        }
    }
    burst_time[min_index] = INT_MAX;
    return min_burst;
}

int main(){
    int process_num;
    scanf("%d", &process_num);
    struct process p[process_num];
    int tmp_burst_time[process_num];

    for (int i = 0; i<process_num; i++){
        int service_time;
        scanf("%d", &service_time);
        tmp_burst_time[i] = service_time;
    }
    for (int j = 0; j < process_num; j++) {
        p[j].bt = sort_shortest_job(process_num, tmp_burst_time);
    }
    //Now The problem is like the previous one (FCFS)
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

}

