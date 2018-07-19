#include <algorithm>
#include <fstream>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ProcessControlBlock.h"
#include "ProcessQueue.h"

#include "RoundRobinScheduler.h"

RoundRobinScheduler::RoundRobinScheduler():
    timeQuantum( 2 ),
    ready(new ProcessQueue())
{}

RoundRobinScheduler::~RoundRobinScheduler()
{
    delete ready;
    ready = nullptr;
}

bool RoundRobinScheduler::Initialize( std::string location )
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
                processId.push_back( pcb->GetProcessId() );
                burstTime.push_back( pcb->GetBurstTime() );
                remainingTime.push_back( pcb->GetBurstTime() );
                arrivalTime.push_back( pcb->GetArrivalTime() );
            }
            currentNode = currentNode->GetNext();
        }
    }

    return success;
}

void RoundRobinScheduler::Execute()
{
    std::vector<int> waitTime;
    std::vector<int> turnAroundTime;

    // Grab first process in the queue
    ProcessQueueNode const *currentNode = ready->GetHead();

    int i = 0;
    int time = 0;
    bool deleteProcess = false;

    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();
        // Make sure we are using the correct PID
        std::vector<int>::iterator it = std::find(processId.begin(), processId.end(), pcb->GetProcessId() );
        
        if ( remainingTime.at(i) > timeQuantum )
        {
            time += timeQuantum;
            PrintGnattChart();
            remainingTime.at(i) -= timeQuantum;
            MoveProcess( *pcb, time );
        }
        else
        {
            // wait time = time - arrivalTime - burstTime
            // turnaround time = time - arrivalTime
            deleteProcess = true;
        }

        currentNode = currentNode->GetNext();
        if ( deleteProcess )
        {
            ready->DeleteProcessFromQueue(pcb->GetProcessId());
            deleteProcess = false;
        }
        i++;
    }
}

bool RoundRobinScheduler::ReadFile( std::string location )
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
        // Sort Ready Queue by ArrivalTime
        list.sort([]( ProcessControlBlock *lhs, ProcessControlBlock *rhs) { return lhs->GetArrivalTime() < rhs->GetArrivalTime(); } );

        for ( auto pcb : list )
        {
            ready->AddProcess(pcb);
        }
    }

    return success;    
}

void RoundRobinScheduler::MoveProcess( ProcessControlBlock &pcb, size_t time )
{
    
}

void RoundRobinScheduler::PrintGnattChart()
{

}
