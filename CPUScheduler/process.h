#ifndef PROCESS_H
#define PROCESS_H

struct Process
{
    wchar_t id[15] = L"";
    int arrivalTime = 0, burstTime = 0;
    int startTime = 0, endTime = 0, remainingTime = 0, waitingTime = 0, turnAroundTime = 0;
    bool finished = false;
};

void sortArrivalTime(Process *processes, int n)
{
    for (int j = 0; j < n - 1; j++)
        for (int i = j + 1; i < n; i++)
            if (processes[j].arrivalTime > processes[i].arrivalTime)
            {
                Process temp = processes[j];
                processes[j] = processes[i];
                processes[i] = temp;
            }
}

int findMinBurst(Process *processes, int n, int cTime)
{
    //findMinBurst dung sau khi mang processes da dc sort tang dan arrivalTime
    int min = INT_MAX, min_i = -1;
    for (int i = 0; i < n; i++)
        if (processes[i].finished == false &&
            processes[i].arrivalTime <= cTime &&
            processes[i].burstTime < min)
        {
            min = processes[i].burstTime;
            min_i = i;
        }    
    return min_i;
}

// int findMinRemaining(Process *processes, int n, int cTime)
// {
//     //findMinRemaining dung sau khi mang processes da dc sort tang dan arrivalTime
//     int min = INT_MAX, min_i = -1;
//     for (int i = 0; i < n; i++)
//         if (processes[i].finished == false &&
//             processes[i].arrivalTime <= cTime &&
//             processes[i].remainingTime < min)
//         {
//             min = processes[i].burstTime;
//             min_i = i;
//         }    
//     return min_i;
// }

void initRemainingTime(Process *processes, int n) //khoi tao remainingTime = burstTime
{
    for (int i = 0; i < n; i++)
        processes[i].remainingTime = processes[i].burstTime;
}

float calcWaitingTime(Process *processes, int n, wchar_t *resultBuff)
{
    float avgWt = 0;
    int cX; // swprintf cursor x
    cX = swprintf_s(resultBuff, 50, L"(");
    ;
    for (int i = 0; i < n; i++)
    {
        avgWt += processes[i].waitingTime;
        cX += swprintf_s(resultBuff + cX, 50 - cX, L"%d", processes[i].waitingTime);
        if (i < n - 1)
            cX += swprintf_s(resultBuff + cX, 50 - cX, L"+");
    }
    avgWt /= n;
    swprintf_s(resultBuff + cX, 50 - cX, L")/%d=%.3f", n, avgWt);
    return avgWt;
}

float calcTurnAroundTime(Process *processes, int n, wchar_t *resultBuff)
{
    float avgTA = 0;
    int cX; // swprintf cursor x
    cX = swprintf_s(resultBuff, 50, L"(");
    for (int i = 0; i < n; i++)
    {
        avgTA += processes[i].turnAroundTime;
        cX += swprintf_s(resultBuff + cX, 50 - cX, L"%d", processes[i].turnAroundTime);
        if (i < n - 1)
            cX += swprintf_s(resultBuff + cX, 50 - cX, L"+");
    }
    avgTA /= n;
    swprintf_s(resultBuff + cX, 50 - cX, L")/%d=%.3f", n, avgTA);
    return avgTA;
}

void resetInfo(Process *processes, int n)
{
    for (int i = 0; i < n; i++)
    {
        processes[i].endTime = processes[i].startTime = processes[i].turnAroundTime = processes[i].waitingTime = 0;
        processes[i].finished = false;
    }
}

#endif