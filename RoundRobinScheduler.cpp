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
    ready(new ProcessQueue()),
    averageWaitTime(0.0),
    averageTurnAroundTime(0.0)
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
                waitTime.push_back(0);
                turnAroundTime.push_back(0);
                processId.push_back( pcb->GetProcessId() );
                burstTime.push_back( pcb->GetBurstTime() );
                priority.push_back( pcb->GetPriorty() );
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
    // Grab first process in the queue
    ProcessQueueNode const *currentNode = ready->GetHead();

    int time = 0;
    bool complete = false;    
    std::vector<int> processTime;
    std::vector<bool> firstPass;
    for ( int i = 0; i < ready->Size(); i++ )
    {
        processTime.push_back(0);
        firstPass.push_back( false );
    }

    std::cout << "" << std::endl;
    std::cout << "Round Robin Results" << std::endl;
    std::cout << "Gnatt Chart" << std::endl;
    while ( currentNode != nullptr )
    {
        ProcessControlBlock *pcb = currentNode->GetData();

        // Make sure we are using the correct PID
        auto it = std::distance( processId.begin(), std::find(processId.begin(), processId.end(), pcb->GetProcessId() ) );

        if ( remainingTime.at(it) > timeQuantum )
        {
            PrintGnattChart( pcb->GetProcessId(), time );
            if ( !firstPass.at(it) )
            {
                waitTime.at(it) += time - arrivalTime.at(it);
                firstPass.at(it) = true;
            }
            else
            {
                waitTime.at(it) += time - processTime.at(it);
            }

            time += timeQuantum;
            remainingTime.at(it) -= timeQuantum;
            MoveProcess( pcb, time );
        }
        else if ( remainingTime.at(it) <= timeQuantum && remainingTime.at(it) > 0 )
        {
            PrintGnattChart( pcb->GetProcessId(), time ); 
            if ( !firstPass.at(it) )
            {
                waitTime.at(it) += time - arrivalTime.at(it);
                firstPass.at(it) = true;                
            }
            else
            {
                waitTime.at(it) += time - processTime.at(it);      
            }     

            time += remainingTime.at(it);
            remainingTime.at(it) = 0;
            complete = true;
        }

        processTime.at(it) = time;

        if ( complete )
        {
            // turnaround time = time
            turnAroundTime.at(it) = time;

            ready->DeleteProcessFromQueue();
            complete = false;
        }

        currentNode = ready->GetHead();

    }

    std::cout << time << std::endl;

    PrintOutput();
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

void RoundRobinScheduler::MoveProcess( ProcessControlBlock *pcb, size_t time )
{
    ready->DeleteProcessFromQueue();

    ProcessQueueNode const *currentNode = ready->GetHead();

    bool done = false;
    while ( currentNode != nullptr && !done )
    {
        if ( currentNode->GetData()->GetArrivalTime() > time )
        {
            ready->AddProcess( pcb, currentNode->GetPrev()->GetData()->GetProcessId() );
            done = true;
        }        

        currentNode = currentNode->GetNext();
    }

    // Arrival time is now less than total time so start adding them to the tail
    if ( !done )
    {
        ready->AddProcess( pcb );
    }
}

void RoundRobinScheduler::PrintOutput()
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

void RoundRobinScheduler::PrintGnattChart( long processId, size_t time )
{
    std::cout << time << "<--" << processId << "-->"; 
}