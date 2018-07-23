#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "FCFSScheduler.h"
#include "ProcessControlBlock.h"
#include "ProcessQueue.h"
#include "PriorityScheduler.h"
#include "RoundRobinScheduler.h"

int main()
{
    ProcessQueue *queue = new ProcessQueue();
    int choice = 0;
    bool run = true;

    while ( run )
    {
        std::cout << "Queue Manipulation" << std::endl;
        std::cout << "1) Add to PID To Queue" << std::endl;
        std::cout << "2) Delete PID From Queue" << std::endl;
        std::cout << "3) Print Queue " << std::endl;
        std::cout << "4) Load PIDs from file " << std::endl;
        std::cout << "5) Exit " << std::endl;
        std::cin >> choice;

        long pid = 0;
        long existing_pid = 0;
        std::string location;
        std::ifstream in;
        FCFSScheduler fcfs;
        PriorityScheduler priority;
        RoundRobinScheduler rr;

        switch ( choice )
        {
            case 1:
                queue->PrintQueue();
                std::cout << "1) Add PID To Tail" << std::endl;
                std::cout << "2) Add PID To Specific Location" << std::endl;
                std::cin >> choice;
                switch( choice )
                {
                    case 1:
                        std::cout << "Enter PID: ";
                        std::cin >> pid;
                        queue->AddProcess(new ProcessControlBlock(pid));
                        queue->PrintQueue();
                    break;

                    case 2:
                        std::cout << "Enter PID: ";
                        std::cin >> pid;
                        std::cout << "After which PID? ";
                        std::cin >> existing_pid;
                        queue->AddProcess( new ProcessControlBlock(pid), existing_pid );
                        queue->PrintQueue();
                    break;
                }
            break;

            case 2:
                queue->PrintQueue();
                std::cout << "1) Delete from head" << std::endl;
                std::cout << "2) Delete Specific PID" << std::endl;
                std::cin >> choice;
                switch( choice )
                {
                    case 1:
                        queue->DeleteProcessFromQueue();
                        queue->PrintQueue();
                    break;

                    case 2:
                        std::cout << "Which PID: ";
                        std::cin >> pid;
                        queue->DeleteProcessFromQueue( pid );
                        queue->PrintQueue();
                    break;
                }
            break;

            case 3:
                queue->PrintQueue();
                std::cout << std::endl;
            break;

            case 4:
                std::cout << "File format should be: Process Id, Arrival Time, Burst Time, Priority" << std::endl;
                std::cout << "File location: ";
                std::cin >> location;
                if ( fcfs.Initialize( location ) )
                {
                    fcfs.Execute();
                }

                if ( priority.Initialize( location ) )
                {
                    priority.Execute();
                }

                if ( rr.Initialize( location ) )
                {
                    rr.Execute();
                }
                
                std::cout << std::endl << std::endl;
                std::cout << std::setw(30) << "Scheduler";
                std::cout << std::setw(30) << "Average Turnaround Time";
                std::cout << std::setw(30) << "Average Wait Time" << std::endl;
                std::cout << std::left << std::setw(30) << "First Come First Serve";
                std::cout << std::left << std::setw(30) << fcfs.GetAverageTurnAroundTime();
                std::cout << std::left << std::setw(30) << fcfs.GetAverageWaitTime() << std::endl;
                std::cout << std::left << std::setw(30) << "Priority";
                std::cout << std::left << std::setw(30) << priority.GetAverageTurnAroundTime();
                std::cout << std::left << std::setw(30) << priority.GetAverageWaitTime() << std::endl;
                std::cout << std::left << std::setw(30) << "Round Robin";
                std::cout << std::left << std::setw(30) << rr.GetAverageTurnAroundTime();
                std::cout << std::left << std::setw(30) << rr.GetAverageWaitTime() << std::endl << std::endl << std::endl;
            break;

            case 5:
                run = false;
            break;

            default:
                run = false;
            break;
        }
    }

    return 0;
}
