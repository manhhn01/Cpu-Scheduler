#define item_list Process

#ifndef CPUS_H

#define CPUS_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"
#include <vector>
#include <queue>

using namespace std;

Process idleP = {L"Idle", 0, 0, 0, 0, -1}; //remaining time = -1

vector<Process> FCFS(Process *processes, int n)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    vector<Process> gantt;
    resetInfo(processes, n);
    sortArrivalTime(processes, n);
    Process prevProcess = idleP;
    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrivalTime > prevProcess.endTime)
        {
            idleP.startTime = prevProcess.endTime;
            idleP.endTime = processes[i].arrivalTime;
            prevProcess = idleP;
            gantt.push_back(prevProcess);
        }
        processes[i].startTime = prevProcess.endTime;
        processes[i].endTime = processes[i].startTime + processes[i].burstTime;
        processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;
        processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;
        gantt.push_back(processes[i]);
        prevProcess = processes[i];
    }
    return gantt;
}

vector<Process> SRTF(Process *processes, int n)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    int cTime = 0,         //Thoi gian hien tai
        countFinished = 0, //Dem so tien trinh da hoan thanh
        minTime,           //Thoi gian con lai nho nhat
        i=-1;
    Process nextProcess, cProcess;
    vector<Process> gantt;
    resetInfo(processes, n);
    initRemainingTime(processes, n);
    sortArrivalTime(processes, n);

    if (processes[0].arrivalTime == 0)
        cProcess = processes[0];
    else
        cProcess = idleP;

    cProcess.startTime = 0;
    nextProcess = idleP;

    while (countFinished < n)
    {
        minTime = INT_MAX;
        for (int j = 0; j < n; j++)
        {
            if ((processes[j].remainingTime < minTime) &&
                (processes[j].arrivalTime <= cTime) &&
                (processes[j].remainingTime > 0))
            {
                nextProcess = processes[j];
                minTime = processes[j].remainingTime;
                i = j;
            }
        }

        if (nextProcess.remainingTime == 0) // for-loop khong tim nextP<=>Khong co tien trinh dang chay hien tai
            nextProcess = idleP;

        if (wcscmp(cProcess.id, nextProcess.id) != 0)
        {
            cProcess.endTime = nextProcess.startTime = cTime;
            gantt.push_back(cProcess);
            cProcess = nextProcess;
        }

        cTime++;
        if (i != -1)
        {
            processes[i].remainingTime--;

            if (processes[i].remainingTime == 0)
            {
                countFinished++;
                processes[i].turnAroundTime = cTime - processes[i].arrivalTime;
                processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
            }
        }
    }
    cProcess.endTime = cTime;
    gantt.push_back(cProcess);
    return gantt;
}

vector<Process> SJF(Process *processes, int n)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    vector<Process> gantt;
    resetInfo(processes, n);
    int i, cTime = 0;
    Process prevProcess = idleP;
    sortArrivalTime(processes, n);
    for (int count = 0; count < n; count++)
    {
        if ((i = findMinBurst(processes, n, cTime)) == -1) // ko tim duoc tien trinh vao cTime
        {
            idleP.startTime = prevProcess.endTime;
            for (int ii = 0; ii < n; ii++) //tim tien trinh ke tiep
                if (processes[ii].finished == false)
                {
                    idleP.endTime = cTime = processes[ii].arrivalTime;
                    break;
                }
            prevProcess = idleP;
            i = findMinBurst(processes, n, cTime);
            gantt.push_back(prevProcess);
        }

        processes[i].startTime = prevProcess.endTime;
        processes[i].endTime = processes[i].startTime + processes[i].burstTime;

        processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;
        processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;

        gantt.push_back(processes[i]);
        prevProcess = processes[i];
        processes[i].finished = true;
        cTime = processes[i].endTime;
    }
    return gantt;
}

vector<Process> RR(Process *processes, int n, int qq)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    vector<Process> gantt;
    queue<int> waitingIndex_Queue; //hang doi chua chi so cua tien trinh
    int i = 0, cTime = 0, countFinished = 0;
    int queuePushed[20] = {0}; //luu trang thai tien trinh trong hang doi
    Process prevProcess = idleP;

    resetInfo(processes, n);
    initRemainingTime(processes, n); //gan RemainingTime = BurstTime
    sortArrivalTime(processes, n);

    while (countFinished < n)
    {
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrivalTime < cTime + qq && //tim tien trinh trong qq tiep
                queuePushed[i] == 0)
            {
                waitingIndex_Queue.push(i);
                queuePushed[i] = 1;
            }
        }

        if (!waitingIndex_Queue.empty())
        {
            i = waitingIndex_Queue.front();
            waitingIndex_Queue.pop();

            if (processes[i].arrivalTime > cTime)
            {
                idleP.startTime = prevProcess.endTime;
                idleP.endTime = processes[i].arrivalTime;
                prevProcess = idleP;
                gantt.push_back(prevProcess);
                cTime = processes[i].arrivalTime;
            }

            processes[i].startTime = prevProcess.endTime;
            if (processes[i].remainingTime > qq)
            {
                processes[i].remainingTime -= qq;
                waitingIndex_Queue.push(i); //push lai vao cuoi queue
                processes[i].endTime = prevProcess.endTime + qq;
                gantt.push_back(processes[i]);
            }
            else //processes[i] ket thuc <=> (remainingTime <= qq)
            {
                processes[i].endTime = prevProcess.endTime + processes[i].remainingTime;
                processes[i].remainingTime = 0;
                gantt.push_back(processes[i]);
                processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;
                processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                countFinished++;
            }
            prevProcess = processes[i];
        }
        else
        {
            idleP.startTime = prevProcess.endTime;
            idleP.endTime = idleP.startTime + qq;
            prevProcess = idleP;
            gantt.push_back(prevProcess);
        }

        cTime = prevProcess.endTime; // cap nhat thoi gian
    }
    return gantt;
}

#endif