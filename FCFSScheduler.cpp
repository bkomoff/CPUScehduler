#include <fstream>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ProcessControlBlock.h"
#include "ProcessQueue.h"

#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler():
    ready(new ProcessQueue()),
    averageWaitTime(0.0),
    averageTurnAroundTime(0.0)
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
                arrivalTime.push_back( pcb->GetArrivalTime() );
                burstTime.push_back(pcb->GetBurstTime());
                priority.push_back( pcb->GetPriorty() );
            }
            currentNode = currentNode->GetNext();
        }
    }

    return success;
}

void FCFSScheduler::Execute()
{
    ProcessQueueNode const *currentNode = ready->GetHead();
    int time = 0;
    std::cout << "" << std::endl;
    std::cout << "First Come First Serve Results" << std::endl;
    std::cout << "Gnatt Chart" << std::endl;
    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();
        if ( pcb != nullptr )
        {
            waitTime.push_back( time - pcb->GetArrivalTime() );
            // turnAroundTime.push_back( ( time + pcb->GetBurstTime() ) - pcb->GetArrivalTime() );
            turnAroundTime.push_back( (time - pcb->GetArrivalTime() ) + pcb->GetBurstTime() );
        }
        
        PrintGnattChart( pcb->GetProcessId(), time );

        time += pcb->GetBurstTime();
        ready->DeleteProcessFromQueue();
        currentNode = ready->GetHead();
    }
    std::cout << time << std::endl;
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
        list.sort([]( ProcessControlBlock *lhs, ProcessControlBlock *rhs) { return lhs->GetArrivalTime() < rhs->GetArrivalTime(); } );

        for ( auto pcb : list )
        {
            ready->AddProcess(pcb);
        }
    }
    
    return success;
}

void FCFSScheduler::PrintOutput()
{
    std::cout << "PID" << std::setw(20);
    std::cout << "Arrival Time" << std::setw(20);
    std::cout << "Burst Time" << std::setw(20);
    std::cout << "Priority" << std::setw(20);
    std::cout << "Wait Time" << std::setw(20);
    std::cout << "Turnaround Time"<< std::setw(20) << std::endl;
    
    for (int i = 0; i < processId.size(); i++ )
    {
        std::cout << std::left << std::setw(20) << processId.at(i);
        std::cout << std::left << std::setw(20) << arrivalTime.at(i);
        std::cout << std::left << std::setw(20) << burstTime.at(i);
        std::cout << std::left << std::setw(20) << priority.at(i);
        std::cout << std::left << std::setw(20) << waitTime.at(i);
        std::cout << std::left << std::setw(20) << turnAroundTime.at(i) << std::endl;

        averageWaitTime       += waitTime.at(i);
        averageTurnAroundTime += turnAroundTime.at(i);
    }

    averageWaitTime       = averageWaitTime / processId.size();
    averageTurnAroundTime = averageTurnAroundTime / processId.size();

    std::cout << "Average Wait Time: " << averageWaitTime << std::endl;
    std::cout << "Average Turnaround Time: " << averageTurnAroundTime << std::endl;
}

void FCFSScheduler::PrintGnattChart( long processId, size_t time )
{
    std::cout << time << "<--" << processId << "-->"; 
}