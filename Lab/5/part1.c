#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    time_t begin, end;

    int hist[12];
    int counter = 0;
    time(&begin);
    for (int j = 0; j < 12; j++) {
        hist[j] = 0;
    }

    srand(time(0));
    int sample_count = 50000;

    for (int i = 0; i < sample_count; i++){
        counter = 0;
        for (int j = 0; j < 12; j++) {
            int r = rand() % 100;
            while (r < 0) {
                r = rand() % 100;
            }
            if (r >= 49) {
                counter++;
            } else {
                counter--;
            }
        }
        hist[counter + 12]++;

    }

    time(&end);

    double time_spent = (double) end - (double) begin;
    printf("%ld\n", time_spent);

    return 0;
}
