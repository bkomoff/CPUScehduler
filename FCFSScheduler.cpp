#include <iostream>
#include <vector>

#include "ProcessControlBlock.h"
#include "ProcessQueue.h"

#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler():
    ready(nullptr)
{}

FCFSScheduler::~FCFSScheduler()
{
    delete ready;
    ready = nullptr;
}

bool FCFSScheduler::Initialize( std::string location )
{   
    bool success = false;

    // Copy to ready queue
    ProcessQueueNode const *currentNode = ready->GetHead();
    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();
        if ( pcb != nullptr )
        {
            pcb->SetProcessState( PCBTypes::ready_process );
        }
        currentNode = currentNode->GetNext();
    }

    return success;
}

void FCFSScheduler::Execute()
{
    std::vector<int> processId;
    std::vector<int> burstTime;
    std::vector<int> waitTime;
    std::vector<int> turnAroundTime;

    ProcessQueueNode const *currentNode = ready->GetHead();
    int i = 0;
    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();
        if ( pcb != nullptr )
        {
            processId.push_back(pcb->GetProcessId());
            burstTime.push_back(pcb->GetBurstTime());
            if ( currentNode->GetPrev() == nullptr )
            {
                waitTime.push_back(0);
            }
            else
            {
                waitTime.push_back( waitTime.at(i - 1) + pcb->GetBurstTime() );
            }

            turnAroundTime.push_back( pcb->GetBurstTime() + waitTime.at(i) );
        }
        i++;
    }


    std::cout << "Process ID     " << "Burst Time     " << "Wait Time     " << "Turnaround Time" << std::endl;

    double averageWaitTime       = 0.0;
    double averageTurnAroundTime = 0.0;
    for (int i = 0; i < processId.size(); i++ )
    {
        std::cout << processId.at(i) << "     " << burstTime.at(i) << "     " << waitTime.at(i) << turnAroundTime.at(i) << std::endl;
        averageWaitTime       += waitTime.at(i);
        averageTurnAroundTime += turnAroundTime.at(i);
    }

    std::cout << "Average Wait Time: " << averageWaitTime / processId.size() << std::endl;
    std::cout << "Average Turnaround Time: " << averageTurnAroundTime / processId.size() << std::endl;
}