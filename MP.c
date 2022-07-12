#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int turnaroundTime;
    int waitingTime;
    int startTime; 
    int endTime;
};

/*
Fuction: sortArrivalTime
sorts the the process list by arrival time

jobs []: list of processes
nProcess: number of process

returns: void
*/
void sortArrivalTime(struct Process jobs[101], int nProcess){
	int i, j;
    struct Process key;
     
    for (i=1; i<nProcess; i++) {
        key = jobs[i];
        j = i-1;
        while (j > -1 && jobs[j].arrivalTime > key.arrivalTime) {
            jobs[j+1] = jobs[j];
            j--;
        }
        jobs[j+1] = key;
    }
}

/*
Fuction: sortBurstTime
sorts the the process list by burst time

jobs []: list of processes
nProcess: number of process

returns: void
*/
void sortBurstTime(struct Process jobs[101], int nProcess){
	int i, j;
    struct Process key;
    
    for (i=2; i<nProcess; i++) {
        key = jobs[i];
        j = i-1;
        while (j > 0 && jobs[j].burstTime > key.burstTime) {
            jobs[j+1] = jobs[j];
            j--;
        }
        jobs[j+1] = key;
    }
}

/*
Fuction: computeAverageWaiting
compute the total waiting time of the process and divide by the number of process

jobs []: list of processes
nProcess: number of process

returns: double
*/
double computeAverageWaiting(struct Process jobs[101], int nProcess){
	int i;
	double avgWait = 0.0;
	for(i = 0; i < nProcess; i++){
		avgWait = avgWait + jobs[i].waitingTime;
	}
	return avgWait / nProcess * 1.0;
}

/*
Fuction: printJobs
prints the start time, end time, waiting time and average time

jobs []: list of processes
nProcess: number of process

returns: void
*/
void printJobs (struct Process jobs[101], int nProcess){
	int i;
	double avgWaiting;
	for(i = 0; i < nProcess; i++){
		printf("P[%d]: Start time: %d \tEnd time: %d \t|  Waiting time: %d\n", jobs[i].processID, jobs[i].startTime, jobs[i].endTime, jobs[i].waitingTime);
	}
	avgWaiting = computeAverageWaiting(jobs, nProcess);
	printf("Average waiting time: %.1f", avgWaiting);
}

/*
Fuction: FCFS
do the first come first serve algorithm 

jobs []: list of processes
nProcess: number of process

returns: void
*/
void FCFS (struct Process jobs[101], int nProcess){
	sortArrivalTime(jobs, nProcess);
	int i = 0;
	int nTime = 0;
	
	for(i = 0; i < nProcess; i++){
		if(nTime >= jobs[i].arrivalTime){
			jobs[i].startTime = nTime;
			nTime = nTime + jobs[i].burstTime;
			jobs[i].endTime = nTime;
		}
		else{
			jobs[i].startTime = jobs[i].arrivalTime;
			nTime  = jobs[i].arrivalTime + jobs[i].burstTime;
		}
		
		jobs[i].turnaroundTime = nTime - jobs[i].arrivalTime;
		jobs[i].waitingTime = jobs[i].turnaroundTime - jobs[i].burstTime;
	}
	
	printJobs(jobs, nProcess);
}

/*
Fuction: SJF
do the shortest job first algorithm 

jobs []: list of processes
nProcess: number of process

returns: void
*/
void SJF (struct Process jobs[101], int nProcess){
	sortBurstTime(jobs, nProcess);
	int i = 0;
	int nTime = 0;
	
	for(i = 0; i < nProcess; i++){
		if(i==0){
			jobs[i].startTime = 0;
			nTime = jobs[i].burstTime;
			jobs[i].endTime = nTime;
		}
		else if(nTime >= jobs[i].burstTime){
			jobs[i].startTime = nTime;
			nTime = nTime + jobs[i].burstTime;
			jobs[i].endTime = nTime;
		}
		else{
			jobs[i].startTime = jobs[i].burstTime;
			nTime  = jobs[i].arrivalTime + jobs[i].burstTime;
		}
		
		jobs[i].turnaroundTime = nTime - jobs[i].arrivalTime;
		jobs[i].waitingTime = jobs[i].turnaroundTime - jobs[i].burstTime;
	}
	
	printJobs(jobs, nProcess);
}

/*
Function: RR
do round robin scheduling

jobs []: list of processes
nProcess: number of processes
slice: limited time slice

returns: void
*/
void RR(struct Process jobs[101], int nProcess, int slice) {
	int i, j, x=0;
	int burstTemp[101];
	int status[101];
	int start[101];
	int done = 0;
	int nTime = 0;

	for (i=0; i<nProcess; i++) {
		burstTemp[i] = jobs[i].burstTime;
		status[i] = 0;
		start[i] = 0;
	}

	while(done == 0) {
		for(i=0; i<nProcess; i++) {
			if (status[i] == 0) {
				if(x==0) {			
					jobs[i].startTime = 0;
						x++;
					if(burstTemp[i] <= slice) {
						jobs[i].endTime = burstTemp[i];
						nTime = burstTemp[i];

						jobs[i].turnaroundTime = nTime - jobs[i].arrivalTime;
						jobs[i].waitingTime = jobs[i].turnaroundTime - jobs[i].burstTime;

						status[i] = 1;
					}
					else {
						jobs[i].endTime = slice;
						burstTemp[i] = jobs[i].burstTime - slice;
						nTime = slice;
					}
				}
				else {
					if(start[i] == 0 && i!=0) {
						jobs[i].startTime = nTime;
					}

					if (burstTemp[i] <= slice) {
						jobs[i].endTime = nTime + burstTemp[i];
						nTime = nTime + burstTemp[i];

						jobs[i].turnaroundTime = nTime - jobs[i].arrivalTime;
						jobs[i].waitingTime = jobs[i].turnaroundTime - jobs[i].burstTime;

						status[i] = 1;
					}
					else {
						jobs[i].endTime = nTime + slice;
						burstTemp[i] = burstTemp[i] - slice;
						nTime = nTime + slice;
					}
				}
				start[i] = 1;
			} 
		} 

		for (j=0; j<nProcess; j++) {
			if(status[j] == 0) {
				done = 0;
				break;
			}
			else
				done = 1;
		}
	}

	printJobs(jobs, nProcess);
}

/*
Function: SRTF
do shortest time remaining first


*/
void SRTF(struct Process jobs[101], int nProcess) {
	int i, j, x=0;
	int burstTemp[101];
	int start[101];
	int done = 0;
	int nTime = 0;
	int key = 0;
	int iter = 0;
	int max;

	for (i=0; i<nProcess; i++) {
		burstTemp[i] = jobs[i].burstTime;
		start[i] = 0;
	}

	while(done != nProcess) {
		i = iter % nProcess;
		max = 256;
		if(burstTemp[i]!=0) {
			//if first job
			if(x==0){
				jobs[i].startTime = 0;
				x++;
				start[i] = 1;
				if(burstTemp[i]==1) {
					jobs[i].endTime = 1;
					nTime = 1;

					jobs[i].turnaroundTime = nTime - jobs[i].arrivalTime;
					jobs[i].waitingTime = jobs[i].turnaroundTime - jobs[i].burstTime;
					
					burstTemp[i] = 0;
					done++;
				} 
				else {
					jobs[i].endTime = 1;
					burstTemp[i] = burstTemp[i] - 1;
					nTime = 1;
				}
				printf("%d, %d\n", i, burstTemp[i]);
			} else {
				//look for shortest burst time
				for(j=0; j<nProcess; j++) {
					if(burstTemp[j]<max && burstTemp[j]!=0) {
						key = j;
						max = burstTemp[j];
					}
				}
				printf("%d, %d\n", key, burstTemp[key]);

				if(start[key] == 0 && key!=0) {
					start[key] = 1;
					jobs[key].startTime = nTime;
				}

				if(burstTemp[key]==1) {
					jobs[key].endTime = nTime + 1;
					nTime = nTime + 1;

					jobs[key].turnaroundTime = nTime - jobs[key].arrivalTime;
					jobs[key].waitingTime = jobs[key].turnaroundTime - jobs[key].burstTime;
					
					burstTemp[key] = 0;
					done++;
				printf("done\n");
				} 
				else {
					jobs[key].endTime = nTime + 1;
					burstTemp[key] = burstTemp[key] - 1;
					nTime = nTime + 1;
				}
			}
		}
		i++;
	}
	printJobs(jobs, nProcess);
}


int main(){
	char fileName[256];
	    FILE *fptr;
	// X Y Z
	int CPUAlgo, nProcess, nRRTSV;
	int i; 
	printf("Enter filename: ");
	scanf("%s", fileName);
	
	fptr = fopen(fileName, "r");
	if(fptr == NULL){
		printf("%s not found.", fileName);
	}
	else{
		fscanf(fptr, "%d %d %d", &CPUAlgo, &nProcess, &nRRTSV);
		struct Process jobs[nProcess];
		
		for(i = 0; i < nProcess; i++){
			fscanf(fptr, "%d %d %d", &jobs[i].processID, &jobs[i].arrivalTime, &jobs[i].burstTime);	
		}
		fclose(fptr);
		
		switch(CPUAlgo){
			case 0: 
				FCFS(jobs, nProcess);
				break;
			case 1:
				SJF(jobs, nProcess);
				break;
			case 2: 
				SRTF(jobs, nProcess);
				break;
			case 3:
				RR(jobs, nProcess, nRRTSV);
				break;
		}
	}
	
	return 0;
}
