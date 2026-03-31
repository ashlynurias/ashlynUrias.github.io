#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define EXIT 0
#define INVALID -1
#define CPU 1
#define PID 5
#define RACE 2
#define BANKER 3
#define PROCESS 5
#define RESOURCE 3
#define TRUE 1
#define FALSE 0
#define MEMORY 4
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3
#define PAGE 5
#define FIFO 0
#define LRU 1
#define FRAMES 4
#define INVALID -1
#define DISK 6
#define FCFS 0
#define SSTF 1
#define REQUEST 8
//global variable shared resource
int resource = 5;

//function prototypes
int displayMenu(void);
//function protoypes for cpu scheduling
void cpuScheduling(void);
void fcfs(int process[], int arrvTime[], int burstTime[]);
void sjf(int process[], int arrvTime[], int burstTime[]);
void displaySchedule(int process[], int arrvTime[], int burstTime[], int wt[], int tat[]);
//function prototypes for race condition
void raceCondition(void);
void* threadFuncOne(void* arg);
void* threadFuncTwo(void* arg);
//function prototypes for banker's algorithm
void bankersAlgorithm(void);
void systemState(int feasable[], int safe[]);
//new function protoypes for memort managment
void memoryManagement(void);
void displayProcess(int allocation[], int processes, int processSize[]);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
//function prototypes for page replacement
void pageReplacement();
void fifo();
void lru();
int findLRU(int time[]);
void displayPages(int page, int allocation[]);
//function prototypes for disk scheduling
void diskScheduling();
void diskFcfs(int request[], int head);
void diskSstf(int request[], int head);

//main
int main() {
    int choice = INVALID;
    while (choice != EXIT) {
        choice = displayMenu();
        switch (choice) {
            case CPU:
                cpuScheduling();
                break;
            case RACE:
                raceCondition();
                break;
          case BANKER:
                bankersAlgorithm();
                break;
          case MEMORY:
                memoryManagement();
                break;
          case PAGE:
                pageReplacement();
                break;
          case DISK:
                diskScheduling();
                break;
            case EXIT:
                exit(EXIT);
                break;
            default:
                break;
        }
    }
    return EXIT;
}

//functions below
int displayMenu(void) {
    int choice = INVALID;
    while (choice == INVALID) {       printf("\n********************Operating System Management Menu********************\n");
        printf("Select the OS program to run, enter the number of your selection.\n");
        printf("1. CPU Scheduling\n");
        printf("2. Race Condition\n");
        printf("3. Bankers Algorithm\n");
        printf("4. Memory Management\n");
        printf("5. Page Replacement\n");
        printf("6. Disk Scheduling\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice < 0 || choice > DISK) {
            choice = INVALID;
        }
    }
    return choice;
}

void cpuScheduling(void) {
    int process[] = {1, 2, 3, 4, 5};
    int arrvTime[] = {0, 2, 4, 6, 7};
    int burstTime[] = {8, 5, 10, 2, 3};
    fcfs(process, arrvTime, burstTime);
    sjf(process, arrvTime, burstTime);
}

void fcfs(int process[], int arrvTime[], int burstTime[]) {
    int wt[PID], tat[PID];

    wt[0] = 0;
    for (int i = 1; i < PID; i++) {
        wt[i] = burstTime[i - 1] + wt[i - 1];
    }
    for (int i = 0; i < PID; i++) {
        tat[i] = burstTime[i] + wt[i];
    }
    displaySchedule(process, arrvTime, burstTime, wt, tat);
}

void sjf(int process[], int arrvTime[], int burstTime[]) {
    int wt[PID], tat[PID];

    for (int i = 0; i < PID - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < PID; j++) {
            if (burstTime[j] < burstTime[idx]) {
                idx = j;
            }
        }

        int temp = burstTime[idx];
        burstTime[idx] = burstTime[i];
        burstTime[i] = temp;

        temp = process[idx];
        process[idx] = process[i];
        process[i] = temp;

        temp = arrvTime[idx];
        arrvTime[idx] = arrvTime[i];
        arrvTime[i] = temp;
    }

    wt[0] = 0; 

    for (int i = 1; i < PID; i++) {
        wt[i] = burstTime[i - 1] + wt[i - 1];
    }

    for (int i = 0; i < PID; i++) {
        tat[i] = burstTime[i] + wt[i];
    }

    displaySchedule(process, arrvTime, burstTime, wt, tat);
}


void displaySchedule(int process[], int arrvTime[], int burstTime[], int wt[], int tat[]) {
    int totWt = 0, totTat = 0;
    float avgWt, avgTat;

    printf("\n************FCFS************\n");
    printf("PID\tAT\tBT\tWT\tTAT\n");
    printf("---\t---\t---\t---\t---\n");
    for (int i = 0; i < PID; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", process[i], arrvTime[i], burstTime[i], wt[i], tat[i]);
        totWt += wt[i];
        totTat += tat[i];
    }

    avgWt = (float)totWt / PID;
    avgTat = (float)totTat / PID;

    printf("\nAverage Waiting time = %.2f\n", avgWt);
    printf("Average turn around time = %.2f\n", avgTat);
}

//functions for the race condition
void raceCondition(void) {
    pthread_t threadOne, threadTwo;
    pthread_create(&threadOne, NULL, threadFuncOne, NULL);
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);
    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);
    printf("Value of shared resource is %d\n", resource);
}

void* threadFuncOne(void* arg) {
    int threadId = 1;
    int fOne = resource;

    printf("Thread %d reads the value of shared resource as %d\n", threadId, resource);
    printf("Local update by Thread %d: %d\n", threadId, ++fOne);
    sleep(1);
    resource = fOne;
    printf("Value of shared resource updated by Thread %d is: %d\n", threadId, resource);
    pthread_exit(NULL);
}

void* threadFuncTwo(void* arg) {
    int threadId = 2;
    int fTwo = resource;

    printf("Thread %d reads the value of shared resource as %d\n", threadId, resource);
    printf("Local update by Thread %d: %d\n", threadId, --fTwo);
    sleep(1);
    resource = fTwo;
    printf("Value of shared resource updated by Thread %d is: %d\n", threadId, resource);
    pthread_exit(NULL);
}
//bankers algorithm functions
void bankersAlgorithm(void) {
    int allocation[PROCESS][RESOURCE] = {{0, 0, 2},
                                         {3, 0, 2},
                                         {0, 1, 0},
                                         {2, 1, 1},
                                         {2, 0, 0}};

    int maxDemand[PROCESS][RESOURCE] = {{4, 3, 3},
                                        {9, 0, 2},
                                        {7, 5, 3},
                                        {2, 2, 2},
                                        {3, 2, 2}};

    int available[RESOURCE] = {2, 4, 6};
    int need[PROCESS][RESOURCE];
    int feasible[PROCESS] = {0};
    int safe[PROCESS];
    int safeIdx = 0;
    for (int p = 0; p < PROCESS; p++) {
        for (int r = 0; r < RESOURCE; r++) {
            need[p][r] = maxDemand[p][r] - allocation[p][r];
        }
    }

    for (int i = 0; i < PROCESS; i++) {
        for (int p = 0; p < PROCESS; p++) {
            if (feasible[p] == FALSE) {
                int isUnsafe = FALSE;
                for (int r = 0; r < RESOURCE; r++) {
                    if (need[p][r] > available[r]) {
                        isUnsafe = TRUE;
                        break;
                    }
                }
                if (isUnsafe == FALSE) {
                    safe[safeIdx++] = p;
                    for (int r = 0; r < RESOURCE; r++) {
                        available[r] += allocation[p][r];
                    }
                    feasible[p] = TRUE;
                }
            }
        }
    }

    systemState(feasible, safe);
}

void systemState(int feasible[], int safe[]) {
    int isSafe = TRUE;

    for (int p = 0; p < PROCESS; p++) {
        if (feasible[p] == FALSE) {
            isSafe = FALSE;
            printf("The operating system is not safe\n");
            break;
        }
    }
    if (isSafe == TRUE) {
        printf("Safe resource allocation to process sequence\n");
        for (int i = 0; i < PROCESS; i++) {
            printf("P%d -> ", safe[i]);
        }
        printf("\n");
    }
}
//memory management functions
void memoryManagement() {
    int blockSize[] = {70, 20, 45, 65, 40, 80};
    int processSize[] = {15, 35, 25, 45, 60, 20};
    int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
    int processes = sizeof(processSize) / sizeof(processSize[0]);
    firstFit(blockSize, blocks, processSize, processes);
    bestFit(blockSize, blocks, processSize, processes);
    worstFit(blockSize, blocks, processSize, processes);
    nextFit(blockSize, blocks, processSize, processes);
}
//first fit
void firstFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }
    printf("*********First Fit*********\n");
    displayProcess(allocation, processes, processSize);
}
//best fit
void bestFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < processes; i++) {
        int bestIdx = -1;
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }
    printf("*********Best Fit*********\n");
    displayProcess(allocation, processes, processSize);
}
//worst fit
void worstFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));
    for (int i = 0; i < processes; i++) {
        int wstIdx = -1;
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (wstIdx == -1 || blockSize[j] > blockSize[wstIdx])
                    wstIdx = j;
            }
        }
        if (wstIdx != -1) {
            allocation[i] = wstIdx;
            blockSize[wstIdx] -= processSize[i];
        }
    }
    printf("*********Worst Fit*********\n");
    displayProcess(allocation, processes, processSize);
}
//next fit
void nextFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));
    int id = 0; 
    for (int i = 0; i < processes; i++) {
        int startingId = id;
        do {
            if (blockSize[id] >= processSize[i]) {
                allocation[i] = id;
                blockSize[id] -= processSize[i];
                break;
            }
            id = (id + 1) % blocks;
        } while (id != startingId);
    }

    printf("*********Next Fit*********\n");
    displayProcess(allocation, processes, processSize);
}

void displayProcess(int allocation[], int processes, int processSize[]) {
    printf("Process No.\tProcess Size\tBlock no.\n");
    for (int i = 0; i < processes; i++) {
        printf("%d\t\t\t%d\t\t\t\t", i + 1, processSize[i]);
        if (allocation[i] != INVALID)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void pageReplacement() {
    printf("\n************Page Replacement************\n\n");
    fifo();
    lru();
}

void fifo() {
    printf("\n************ First In First Out ************\n");
  printf("Page   Frame 1   Frame 2   Frame 3   Frame 4\n");
    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    int allocation[FRAMES] = {0};
    int pageFaults = 0;
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
    int front = 0, rear = 0;
    int queue[FRAMES] = {-1};

    for (int i = 0; i < pages; i++) {
        int present = 0;
        for (int j = 0; j < FRAMES; j++) {
            if (allocation[j] == pageRequests[i]) {
                present = 1;
                break;
            }
        }
        if (!present) {
            if (rear < FRAMES) {
                allocation[rear] = pageRequests[i];
                queue[rear] = pageRequests[i];
                rear++;
            } else {
                allocation[front] = pageRequests[i];
                queue[front] = pageRequests[i];
                front = (front + 1) % FRAMES;
            }
            pageFaults++;
        }

        printf("   %d\t\t", pageRequests[i]);
      for (int j = 0; j < FRAMES; j++) {
            if (allocation[j] == 0) {
                printf("-  \t\t");
            } 
            else {
                printf("%d  \t\t", allocation[j]);
            }
          }
          printf("\n");
      }
    printf("Total Page Faults: %d\n", pageFaults);
}

void lru() {
    printf("\n************ Least Recently Used  ************\n");
  printf("Page   Frame 1   Frame 2   Frame 3   Frame 4\n");
  int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
  int allocation[FRAMES] = {0};
  int pageFaults = 0;
  int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);

  int time[FRAMES];
  memset(time, -1, sizeof(time));
  int counter = 0;

  for (int i = 0; i < pages; i++) {
      int flag1 = 0, flag2 = 0;
      for (int j = 0; j < FRAMES; j++) {
          if (allocation[j] == pageRequests[i]) {
              counter++;
              time[j] = counter;
              flag1 = flag2 = 1;
              break;
          }
      }
      if (flag1 == 0) {
          int pos = findLRU(time);
          allocation[pos] = pageRequests[i];
          counter++;
          time[pos] = counter;
          pageFaults++;
      }
        printf("   %d\t\t", pageRequests[i]);
        for (int j = 0; j < FRAMES; j++) {
          if (allocation[j] == 0) {
              printf("-  \t\t");
          } 
          else {
              printf("%d  \t\t", allocation[j]);
          }
        }
        printf("\n");
    }

    printf("Total Page Faults = %d\n", pageFaults);
}

int findLRU(int time[]) {
    int min = time[0], pos = 0;
    for (int i = 1; i < FRAMES; i++) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}
//disk scheduling functions
void diskScheduling() {
    int requests[] = {146, 89, 24, 70, 102, 13, 51, 134};
    int head = 50;

    printf("********Disk Scheduling********\n");

    printf("********FCFS********\n");
    diskFcfs(requests, head);

    printf("\n********SSTF********\n");
    diskSstf(requests, head);
}

void diskFcfs(int requests[], int head) {
    int seek = 0;
    printf("Seek Sequence: %d -> ", head);
    for (int i = 0; i < REQUEST; i++) {
        seek += abs(head - requests[i]);
        printf("%d -> ", requests[i]);
        head = requests[i];
    }
    printf("\nTotal seek operations: %d\n", seek);
}

void diskSstf(int requests[], int head) {
    int seek = 0;
    int minVal, minValIdx;
    int distance[REQUEST];
    int sequence[REQUEST];
    int start = head;
    printf("Seek Sequence: %d -> ", head);
    for (int i = 0; i < REQUEST; i++) {
        distance[i] = abs(head - requests[i]);
    }
    for (int i = 0; i < REQUEST; i++) {
        minVal = 999;
        for (int j = 0; j < REQUEST; j++) {
            if (distance[j] < minVal) {
                minVal = distance[j];
                minValIdx = j;
            }
        }
        sequence[i] = requests[minValIdx];
        distance[minValIdx] = 999; 
        seek += abs(start - sequence[i]);
        start = sequence[i]; 
        printf("%d", sequence[i]);
        if (i != REQUEST - 1) {
            printf(" -> ");
        }
    }
    printf("\nTotal seek operations: %d\n", seek);
}

