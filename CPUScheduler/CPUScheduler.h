#define item_list Process

#ifndef CPUS_H

#define CPUS_H
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"
#include "list.h"
#include "queue.h"

Process idleP = {L"Idle", 0, 0, 0, 0, -1}; //remaining time = -1

List FCFS(Process *processes, int n)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    List gantt = NULL;
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
            listAdd(&gantt, prevProcess);
        }
        processes[i].startTime = prevProcess.endTime;
        processes[i].endTime = processes[i].startTime + processes[i].burstTime;
        processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;
        processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;
        listAdd(&gantt, processes[i]);
        prevProcess = processes[i];
    }
    return gantt;
}

List SRTF(Process *processes, int n)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    int
        cTime = 0,         //Thoi gian hien tai
        countFinished = 0, //Dem so tien trinh da hoan thanh
        minTime,
        prevTime = 0;
    Process
        *nextProcess,
        cProcess;

    List gantt = NULL;
    resetInfo(processes, n);
    initRemainingTime(processes, n);
    sortArrivalTime(processes, n);

    if (processes[0].arrivalTime == 0)
        cProcess = processes[0];
    else
        cProcess = idleP;

    cProcess.startTime = 0;
    nextProcess = &idleP;

    while (countFinished < n)
    {
        minTime = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if ((processes[i].remainingTime < minTime) &&
                (processes[i].arrivalTime <= cTime) &&
                (processes[i].remainingTime > 0))
            {
                nextProcess = &processes[i];
                minTime = processes[i].remainingTime;
            }
        }

        if (nextProcess->remainingTime == 0) //for-loop khong tim nextP<=>Khong co tien trinh dang chay hien tai
            nextProcess = &idleP;

        if (wcscmp(cProcess.id, nextProcess->id) != 0)
        {
            cProcess.endTime = nextProcess->startTime = cTime;
            listAdd(&gantt, cProcess);
            cProcess = *nextProcess; // luu lai cho while tiep theo
        }
        cTime++;
        nextProcess->remainingTime--; // ~ cProcess.remainingTime;

        if (nextProcess->remainingTime == 0)
        {
            countFinished++;
            nextProcess->turnAroundTime = cTime - nextProcess->arrivalTime;
            nextProcess->waitingTime = nextProcess->turnAroundTime - nextProcess->burstTime;
        }
    }
    cProcess.endTime = cTime;
    listAdd(&gantt, cProcess);
    return gantt;
}

List SJF(Process *processes, int n)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    List gantt = NULL;
    resetInfo(processes, n);
    int i, cTime = 0;
    Process prevProcess = idleP;
    sortArrivalTime(processes, n);
    for (int count = 0; count < n; count++)
    {
        if ((i = findMinBurst(processes, n, cTime)) == -1) // ko tim duoc tien trinh vao cTime
        {
            idleP.startTime = prevProcess.endTime;
            for (int ii = 0; ii < n; ii++)
                if (processes[ii].finished == false)
                {
                    idleP.endTime = cTime = processes[ii].arrivalTime;
                    break;
                }
            prevProcess = idleP;
            i = findMinBurst(processes, n, cTime);
            listAdd(&gantt, prevProcess);
        }

        processes[i].startTime = prevProcess.endTime;
        processes[i].endTime = processes[i].startTime + processes[i].burstTime;

        processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;
        processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;

        listAdd(&gantt, processes[i]);
        prevProcess = processes[i];
        processes[i].finished = true;
        cTime = processes[i].endTime;
    }
    return gantt;
}

List RR(Process *processes, int n, int qq)
{
    idleP = {L"Idle", 0, 0, 0, 0, -1};
    List gantt = NULL;
    Queue waitingIndexQueue;
    resetInfo(processes, n);
    initRemainingTime(processes, n);
    sortArrivalTime(processes, n);
    int i = 0, cTime = 0, countFinished = 0;
    int queuePushed[20] = {0}; //luu tien trinh da them vao hang doi
    Process prevProcess = idleP;
    while (countFinished < n)
    {
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrivalTime < cTime + qq && //tim tien trinh trong qq tiep
                queuePushed[i] == 0)
            {
                queuePush(&waitingIndexQueue, i);
                queuePushed[i] = 1;
            }
        }
        i = queuePop(&waitingIndexQueue);
        if (i == -1)
        {
            idleP.startTime = prevProcess.endTime;
            idleP.endTime = prevProcess.startTime + qq;
            prevProcess = idleP;
            listAdd(&gantt, prevProcess);
        }
        else //co tien trinh trong hang doi
        {

            if (processes[i].arrivalTime > cTime)
            {
                idleP.startTime = prevProcess.endTime;
                idleP.endTime = processes[i].arrivalTime;
                prevProcess = idleP;
                listAdd(&gantt, prevProcess);
                cTime = processes[i].arrivalTime;
            }

            processes[i].startTime = prevProcess.endTime;
            if (processes[i].remainingTime > qq)
            {
                processes[i].remainingTime -= qq;
                queuePush(&waitingIndexQueue, i); //push lai vao cuoi queue
                processes[i].endTime = prevProcess.endTime + qq;
                listAdd(&gantt, processes[i]);
            }
            else //processes[i] ket thuc <=> remainingTime <= qq
            {
                processes[i].endTime = prevProcess.endTime + processes[i].remainingTime;
                processes[i].remainingTime = 0;
                listAdd(&gantt, processes[i]);
                processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;
                processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                countFinished++;
            }
            prevProcess = processes[i];
        }
        cTime = prevProcess.endTime; // cap nhat thoi gian
    }
    return gantt;
}

#endif