#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include <string.h>
#include<sys/wait.h>

int main() {
    int fd1[2];
    int fd2[2];
    char input_str[100];
    pid_t p;

    if (pipe(fd1) == -1) {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd2) == -1) {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }

    gets(input_str);
    p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed" );
        return 1;
    } // Parent process
    else if (p > 0) {
        char output_str[100];
        close(fd1[0]);
        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]);
        wait(NULL);
        close(fd2[1]);
        read(fd2[0], output_str, 100);
        printf("Output string is:\n%s\n", output_str);
        close(fd2[0]);
    }// child process
    else {
        close(fd1[1]);
        char input[100];
        read(fd1[0], input, 100);
        char up_down_str[100];
        int i;
        for (i = 0; i < strlen(input); i++) {
            if (isupper(input[i])){
                up_down_str[i] = tolower(input[i]);
            } else {
                up_down_str[i] = toupper(input[i]);
            }
        }
        up_down_str[i] = '\0';
        close(fd1[0]);
        close(fd2[0]);
        write(fd2[1], up_down_str, strlen(up_down_str)+1);
        close(fd2[1]);
        exit(0);
    }
}
