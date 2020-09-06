#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_CHAR 128;
#define MAX_ARG 32;

struct Process {
    int pid;
    char* name;
    long startingTime;
    struct Process* last;
    struct Process* next;
}

Process* head;
Process* tail;
int size = 0;

void addProcess(struct Process* add) {
    if(size == 0){
        head = add;
        tail = add;
    }
    else {
        add -> next = tail;
        tail -> next = add;
        tail = add;
    }
    size++;
}

void removeProcess(struct Process* remove) {
    if(size == 1) {
        tail = NULL;
        head = NULL;
        size--;
    }
    else if(size == 0) {
        printf("There is no process to remove.");
    }
    else {
        if(remove -> next) {    // remove is the first one
            remove -> next -> last = NULL;
            head = remove -> next;
        }
        else if(remove -> last) {   // remove is the last one
            remove -> last -> next = NULL;
            tail = remove -> last;
        }
        else if(remove -> last && remove -> next) {
            remove -> last -> next = remove -> next;
            remove -> next -> last = remove -> last;
         }
         size--;
    }
}

long convertTime(struct timeval time) {
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

void displayStats(struct timeval start) {
    struct rusage usageStats;
    struct timeval end;
    gettimeofdat(&end, NULL);
    getrusage(RUSAGE_SELF, &usageStats);

    long int startTime = convertTime(start);
    long int endTime = convertTime(end);

    printf("Displaying Statistics...");
    printf("CPU time used for user: %ldms\n", convertTime(usageStats.ru_utime));
    printf("CPU time used for system: %ldms\n", convertTime(usageStats.ru_stime));
    printf("Elapsed \"wall-clock\" time for the command to execute: %ldms\n", endTime - startTime);
    printf("Number of times the process was preempted involuntarily: %ld\n", usageStats.ru_nivcsw);
    printf("Number of times the process gave up the CPU voluntarily: %ld\n", usageStats.ru_nvcsw);
    printf("Number of major page faults that require disk I/O: %ld\n", usageStats.ru_majflt);
    printf("Number of major page faults that can be satisfied without disk I/O: %ld\n", usageStats.ru_minflt);
}

void execute(char* arg[], int back) {
    
}

int main(int argc, char *argv[]) {
    char* prompt = "==>";
    char* arg[MAX_ARG];

    printf("Starting program...");

    if(argc == 1) {
        printf("Shell Program");
        printf("%s", prompt);

        char* input = malloc(sizeof(char) * MAX_CHAR);

        while(1) {
            fgets(input, MAX_CHAR, stdin);
            
            input =strtok(input, "\n");
            parseInput(arg, input);

            if(strcmp(arg[0], "exit") == 0) {
                printf("Exited");
                exit(0);
            }
            else if(strcmp(arg[0], "cd") == 0) {
                if(arg[1]) {
                    chdir(arg[1]);
                }
            }
            else if(strcmp(arg[0], "set") == 0 && strcmp(arg[1], "prompt") == 0 && 
                    strcmp(arg[2], "=") && arg[3]) {
                prompt = arg[3];
                printf("Prompt set to %s\n", prompt);
            }
            else if(strcmp(arg[0], jobs) == 0) {
                int i = 0;
                struct backgroundProcess* crt = head;
                if(crt) {
                    while(i <= size) {
                        printf("[%d] %d %s\n", i, crt -> pid, crt -> name);
                        crt = crt -> next;
                        i++;
                    }
                }
            }
            else {
                printf("Regular commands");
                int i = 0;
                while(arg[i]) {
                    i++；
                }
                if(strcmp(arg[i - 1], "&") == 0) {
                    arg[i - 1] = NULL;
                    execute(arg, 1);
                }
                else {
                    execute(arg, 0);
                }
            }
            printf("%s", prompt);
        }
        else {
            execute(argv + 1, 0);
        }
    }
}