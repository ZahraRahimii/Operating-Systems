#include <stdio.h>
#include <math.h>
#define INT_MAX 2147483647

struct process{
    int pid;
    int bt;
    int wt, last_ex;
};

int main(){
    int process_num;
    scanf("%d", &process_num);
    struct process p[process_num];
    int burst_sum = 0;
    for (int i = 0; i<process_num; i++){
        int service_time;
        scanf("%d", &service_time);
        burst_sum += service_time;
        p[i].bt = service_time;
        p[i].pid = i;
        p[i].wt = 0;
        p[i].last_ex=0;
    }
    int q;
    scanf("%d", &q);

    int num = (int) ceil((double )burst_sum/q);
    int k = 0;
    int time = 0;
    for (int j = 0; j < num; j++) {
        if (k >= process_num)
            k = 0;
        if (p[k].bt <= q) {
            printf("Process %d has finished\n", p[k].pid);
            p[k].wt = p[k].wt + time - p[k].last_ex;
            p[k].last_ex = time + p[k].bt;
            p[j].bt = INT_MAX;
            time = p[k].last_ex;
        } else if (p[k].bt != INT_MAX){
            p[k].bt = p[k].bt - q;
            p[k].wt = p[k].wt + time - p[k].last_ex;
            printf("Process %d is executing\n", p[k].pid);
            p[k].last_ex = time + q;
            time = p[k].last_ex;
        } else
            j--;
        k++;
    }
    printf("\n");
    for (int l = 0; l < process_num; ++l) {
        printf("Process %d has waited %d seconds\n", p[l].pid, p[l].wt);
    }
    int avg_waiting = 0;
    for (int m = 0; m < process_num; ++m) {
        avg_waiting += p[m].wt;
    }
    printf("\nTotal Average Waiting is %.2f", (double )avg_waiting/process_num);
    return 0;
}