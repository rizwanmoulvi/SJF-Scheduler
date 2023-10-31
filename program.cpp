#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int completedTime;
    int turnAroundTime;
    int waitingTime;
};

// Function to add a process to the queue based on arrival time
void addToQueue(struct Process queue[], int *queueSize, struct Process processList[], int *processListSize, int time) {
    for (int i = 0; i < *processListSize; i++) {
        if (processList[i].arrivalTime <= time) {
            queue[*queueSize] = processList[i];
            (*queueSize)++;
            for (int j = i; j < (*processListSize - 1); j++) {
                processList[j] = processList[j + 1];
            }
            (*processListSize)--;
            i--;
        }
    }
}

// Function to select the shortest burst time process from the queue
struct Process selectProcess(struct Process queue[], int *queueSize) {
    int minBurstTime = queue[0].burstTime;
    int minIndex = 0;

    for (int i = 1; i < *queueSize; i++) {
        if (queue[i].burstTime < minBurstTime) {
            minBurstTime = queue[i].burstTime;
            minIndex = i;
        }
    }

    struct Process process = queue[minIndex];

    for (int i = minIndex; i < (*queueSize - 1); i++) {
        queue[i] = queue[i + 1];
    }
    (*queueSize)--;

    return process;
}

int main() {
    int time = 0;
    int totalProcessingTime = 0; // Initialize total processing time
    int n; // Number of processes
    int processListSize = 0;
    struct Process processList[100];
    struct Process queue[100];
    int queueSize = 0;

    // Initialize completedList
    struct Process completedList[100];
    int completedListSize = 0;

    // Get input for processList
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Populate processList with process data
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time for process P%d: ", i + 1);
        scanf("%d", &processList[processListSize].arrivalTime);
        printf("Enter burst time for process P%d: ", i + 1);
        scanf("%d", &processList[processListSize].burstTime);
        processList[processListSize].processID = i + 1;
        processListSize++;
    }

    while (processListSize > 0 || queueSize > 0) {
        addToQueue(queue, &queueSize, processList, &processListSize, time);

        while (queueSize == 0) {
            time += 2; // Add a 2-time unit overhead
            addToQueue(queue, &queueSize, processList, &processListSize, time);
        }

        struct Process processToRun = selectProcess(queue, &queueSize);

        // Simulate the execution of the selected process
        for (int i = 0; i < processToRun.burstTime; i++) {
            time++;
            totalProcessingTime++; // Update total processing time
            addToQueue(queue, &queueSize, processList, &processListSize, time);
        }

        // Add a 2-time unit overhead
        time += 2;
        totalProcessingTime += 2; // Update total processing time

        // Update process data and add it to the completedList
        processToRun.completedTime = time;
        processToRun.turnAroundTime = processToRun.completedTime - processToRun.arrivalTime;
        processToRun.waitingTime = processToRun.turnAroundTime - processToRun.burstTime;
        completedList[completedListSize] = processToRun;
        completedListSize++;
    }

    // Output the completedList
    printf("Process\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");

    for (int i = 0; i < completedListSize; i++) {
        struct Process process = completedList[i];
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", process.processID, process.arrivalTime, process.burstTime, process.completedTime-2, process.waitingTime-2, process.turnAroundTime-2);
    }

    // Calculate and print the total processing time
    printf("Total Processing Time: %d\n", totalProcessingTime-2);

    // Calculate and print the average turnaround time, average waiting time, and throughput
    int maxCompletedTime = 0;
    int avgTurnaroundTime = 0;
    int avgWaitingTime = 0;

    for (int i = 0; i < completedListSize; i++) {
        if (completedList[i].completedTime > maxCompletedTime) {
            maxCompletedTime = completedList[i].completedTime;
        }
        avgTurnaroundTime += completedList[i].turnAroundTime;
        avgWaitingTime += completedList[i].waitingTime;
    }

    printf("Average Turnaround Time: %.2f\n", (float)avgTurnaroundTime / completedListSize);
    printf("Average Waiting Time: %.2f\n", (float)avgWaitingTime / completedListSize);
    printf("Throughput: %.2f\n", (float)completedListSize / maxCompletedTime);

    return 0;
}
