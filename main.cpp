#include <fstream>
#include <iostream>
#include <list>

#include "ProcessQueue.h"

int main()
{
    ProcessQueue *readyQueue = new ProcessQueue();
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
        std::string location("");
        std::ifstream in;
        switch ( choice )
        {
            case 1:
                readyQueue->PrintQueue();
                std::cout << "1) Add PID To Tail" << std::endl;
                std::cout << "2) Add PID To Specific Location" << std::endl;
                std::cin >> choice;
                switch( choice )
                {
                    case 1:
                        std::cout << "Enter PID: ";
                        std::cin >> pid;
                        readyQueue->AddProcess( pcb );
                        readyQueue->PrintQueue();
                    break;

                    case 2:
                        std::cout << "Enter PID: ";
                        std::cin >> pid;
                        std::cout << "After which PID? ";
                        std::cin >> existing_pid;
                        readyQueue->AddProcess( existing_pid, pid );
                        readyQueue->PrintQueue();
                    break;
                }
            break;

            case 2:
                readyQueue->PrintQueue();
                std::cout << "1) Delete from head" << std::endl;
                std::cout << "2) Delete Specific PID" << std::endl;
                std::cin >> choice;
                switch( choice )
                {
                    case 1:
                        readyQueue->DeleteProcessFromQueue();
                        readyQueue->PrintQueue();
                    break;

                    case 2:
                        std::cout << "Which PID: ";
                        std::cin >> pid;
                        readyQueue->DeleteProcessFromQueue( pid );
                        readyQueue->PrintQueue();
                    break;
                }
            break;

            case 3:
                readyQueue->PrintQueue();
                std::cout << std::endl;
            break;

            case 4:
                std::cout << "File format should be: Process Id, Arrival Time, Burst Time, Priority" << std::endl;
                std::cout << "File location: ";
                std::cin >> location;
                in.open( location, std::ifstream::in );
                if ( in.is_open() )
                {
                    while ( in.good() )
                    {

                    }
                }
                else
                {
                    std::cout << "File not found" << std::endl;
                }

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
