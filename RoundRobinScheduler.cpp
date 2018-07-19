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

    ProcessQueueNode const *currentNode = ready->GetHead();
    int i = 0;
    int time = 0;
    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();
        size_t currentBurstTime = pcb->GetBurstTime(); 
        if ( currentBurstTime > timeQuantum )
        {
            time += timeQuantum;
            pcb->SetBurstTime( currentBurstTime - timeQuantum );
            MoveProcess( *pcb, time );
        }
        else
        {
        }

        currentNode = currentNode->GetNext();
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
        // Sort Ready Queue by Priority
        list.sort([]( ProcessControlBlock *lhs, ProcessControlBlock *rhs) { return lhs->GetArrivalTime() < rhs->GetArrivalTime(); } );

        for ( auto pcb : list )
        {
            ready->AddProcess(pcb);
            processId.push_back( pcb->GetProcessId() );
            burstTime.push_back( pcb->GetBurstTime() );
        }
    }

    return success;    
}

void RoundRobinScheduler::MoveProcess( ProcessControlBlock &pcb, size_t time )
{

}
