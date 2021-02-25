#include "CPUScheduler.h"
#include <stdio.h>
#include <iostream>
#include <wchar.h>

using namespace std;

int main()
{
    List tempList;
    // tempList = SRTF(processes, 2);

    // for(int i=0; i< listLength(tempList);i++)
    // {
    //     wcout <<listGet(tempList, i).id;
    // }
    // wcout<<endl;
    // for(int i=0; i< listLength(tempList);i++)
    // {
    //     wcout <<listGet(tempList, i).endTime;
    // }
    Process processes[] = {{L"p1", 1, 2, 0, 0}, {L"p2", 4, 3, 0, 0}};
    tempList = FCFS(processes, 2);

    for(int i=0; i< listLength(tempList);i++)
    {
        wcout <<listGet(tempList, i).id;
    }
    wcout<<endl;
    for(int i=0; i< listLength(tempList);i++)
    {
        wcout <<listGet(tempList, i).endTime;
    }
}