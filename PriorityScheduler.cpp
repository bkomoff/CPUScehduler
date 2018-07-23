#include <algorithm>
#include <fstream>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ProcessControlBlock.h"
#include "ProcessQueue.h"

#include "PriorityScheduler.h"

PriorityScheduler::PriorityScheduler():
    ready(new ProcessQueue()),
    averageWaitTime(0.0),
    averageTurnAroundTime(0.0)
{}

PriorityScheduler::~PriorityScheduler()
{
    delete ready;
    ready = nullptr;
}

bool PriorityScheduler::Initialize( std::string location )
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
                arrivalTime.push_back( pcb->GetArrivalTime() );
                priority.push_back( pcb->GetPriorty() );
                burstTime.push_back( pcb->GetBurstTime() );
                waitTime.push_back(0);
                turnAroundTime.push_back(0);
            }

            currentNode = currentNode->GetNext();
        }
    }

    return success;
}

void PriorityScheduler::Execute()
{
    int time = 0;

    std::cout << "" << std::endl;
    std::cout << "Priority Scheduler (1 is highest priority)" << std::endl;
    std::cout << "Gnatt Chart" << std::endl;
    while ( ready->Size() > 0 )
    {
        //Check which PCB has the highest priority in this time frame 
        std::list<const ProcessControlBlock*> priorityQueue;
        for ( size_t i  = 0; i < ready->Size(); i++ )
        {
            if ( ready->Get( i )->GetArrivalTime() <= time )
            {
                priorityQueue.push_back( ready->Get( i ) );
            }
        }      

        // Now sort based on priority
        priorityQueue.sort([]( const ProcessControlBlock *lhs, const ProcessControlBlock *rhs) { return lhs->GetPriorty() > rhs->GetPriorty(); } );

        for (auto pcb : priorityQueue )
        {
            // Make sure we are using the correct PID
            auto it = std::distance( processId.begin(), std::find(processId.begin(), processId.end(), pcb->GetProcessId() ) );
            
            waitTime.at(it) = time - arrivalTime.at(it);
            turnAroundTime.at(it) = ( time - arrivalTime.at(it) ) + burstTime.at(it);

            PrintGnattChart( pcb->GetProcessId(), time );
            time += pcb->GetBurstTime();
            ready->DeleteProcessFromQueue( pcb->GetProcessId() );
        }

        priorityQueue.clear();
    }

    std::cout << time << std::endl << std::endl << std::endl;
    PrintOutput();
}

bool PriorityScheduler::ReadFile( std::string location )
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

void PriorityScheduler::PrintOutput()
{
    std::cout << std::setw(15) << "PID";
    std::cout << std::setw(15) << "Arrival Time";
    std::cout << std::setw(15) << "Burst Time";
    std::cout << std::setw(15) << "Priority";
    std::cout << std::setw(15) << "Wait Time";
    std::cout << std::setw(15) << "Turnaround Time" << std::endl;

    for (int i = 0; i < processId.size(); i++ )
    {
        std::cout << std::left << std::setw(15) << processId.at(i);
        std::cout << std::left << std::setw(15) << arrivalTime.at(i);
        std::cout << std::left << std::setw(15) << burstTime.at(i);
        std::cout << std::left << std::setw(15) << priority.at(i);
        std::cout << std::left << std::setw(15) << waitTime.at(i);
        std::cout << std::left << std::setw(15) << turnAroundTime.at(i) << std::endl;

        averageWaitTime       += waitTime.at(i);
        averageTurnAroundTime += turnAroundTime.at(i);
    }

    averageWaitTime       = averageWaitTime / processId.size();
    averageTurnAroundTime = averageTurnAroundTime / processId.size();

    std::cout << "Average Wait Time: " << averageWaitTime << std::endl;
    std::cout << "Average Turnaround Time: " << averageTurnAroundTime << std::endl;
}

void PriorityScheduler::PrintGnattChart( long processId, size_t time )
{
    std::cout << time << "<--" << processId << "-->"; 
}