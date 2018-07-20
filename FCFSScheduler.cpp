#include <fstream>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ProcessControlBlock.h"
#include "ProcessQueue.h"

#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler():
    ready(new ProcessQueue())
{}

FCFSScheduler::~FCFSScheduler()
{
    delete ready;
    ready = nullptr;
}

bool FCFSScheduler::Initialize( std::string location )
{   
    bool success = ReadFile( location );

    if ( success )
    {
        ProcessQueueNode const *currentNode = ready->GetHead();
        while ( currentNode != nullptr )
        {
            ProcessControlBlock *pcb = currentNode->GetData();
            if ( pcb != nullptr )
            {
                pcb->SetProcessState( PCBTypes::ready_process );
                processId.push_back(pcb->GetProcessId());
                priority.push_back( pcb->GetPriorty() );
                burstTime.push_back(pcb->GetBurstTime());
            }
            currentNode = currentNode->GetNext();
        }
    }

    return success;
}

void FCFSScheduler::Execute()
{
    ProcessQueueNode const *currentNode = ready->GetHead();
    int i = 0;
    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();
        if ( pcb != nullptr )
        {
            if ( i == 0 )
            {
                waitTime.push_back(0);
            }
            else
            {
                waitTime.push_back( waitTime.at(i - 1) + pcb->GetBurstTime() );
            }

            turnAroundTime.push_back( pcb->GetBurstTime() + waitTime.at(i) );
        }

        currentNode = currentNode->GetNext();
        ready->DeleteProcessFromQueue( pcb->GetProcessId() );
        i++;
    }

    PrintOutput();
}

bool FCFSScheduler::ReadFile( std::string location )
{
    bool success = false;

    std::list<ProcessControlBlock*> list;
    std::ifstream file;
    file.open( location, std::ifstream::in );
    if ( file.is_open() )
    {
        std::string line;
        while ( getline( file, line ) )
        {
            std::stringstream linestream( line );
            std::string       value;
            int i = 0;
            int number[4] = {0};

            while ( getline( linestream, value, ',' ) )
            {
                number[i] = stoi(value);
                i++;
            }
        
            ProcessControlBlock *pcb = new ProcessControlBlock( number[0] );
            pcb->SetArrivalTime( number[1] );
            pcb->SetBurstTime( number[2] );
            pcb->SetPriorty( number[3] );

            list.push_back( pcb );
        }

        success = true;
    }

    if ( success )
    {
        // Sort Ready Queue by Priority
        list.sort([]( ProcessControlBlock *lhs, ProcessControlBlock *rhs) { return lhs->GetPriorty() < rhs->GetPriorty(); } );

        for ( auto pcb : list )
        {
            ready->AddProcess(pcb);
        }
    }
    
    return success;
}

void FCFSScheduler::PrintOutput()
{
    std::cout << "" << std::endl;
    std::cout << "First Come First Serve Results" << std::endl;
    std::cout << "PID" << std::setw(20);
    std::cout << "Priority" << std::setw(20);
    std::cout << "Burst Time" << std::setw(20);
    std::cout << "Wait Time" << std::setw(20);
    std::cout << "Turnaround Time" << std::endl;

    double averageWaitTime       = 0.0;
    double averageTurnAroundTime = 0.0;
    
    for (int i = 0; i < processId.size(); i++ )
    {
        std::cout << std::left << std::setw(20) << processId.at(i);
        std::cout << std::left << std::setw(20) << priority.at(i);
        std::cout << std::left << std::setw(20) << burstTime.at(i);
        std::cout << std::left << std::setw(20) << waitTime.at(i);
        std::cout << std::left << std::setw(20) << turnAroundTime.at(i) << std::endl;

        averageWaitTime       += waitTime.at(i);
        averageTurnAroundTime += turnAroundTime.at(i);
    }

    std::cout << "Average Wait Time: " << averageWaitTime / processId.size() << std::endl;
    std::cout << "Average Turnaround Time: " << averageTurnAroundTime / processId.size() << std::endl;
}