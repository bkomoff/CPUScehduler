#pragma once

#include <vector>

class ProcessQueue;

class RoundRobinScheduler
{
    public:
        RoundRobinScheduler();
        ~RoundRobinScheduler();

        bool Initialize( std::string location );
        void Execute();
        double GetAverageWaitTime() const { return averageWaitTime; }
        double GetAverageTurnAroundTime() const { return averageTurnAroundTime; }

    private:
        bool ReadFile( std::string location );
        void MoveProcess( ProcessControlBlock *pcb, size_t time );
        void PrintOutput();
        void PrintGnattChart( long processId, size_t time );

    private:
        const size_t timeQuantum;
        ProcessQueue *ready;
        std::vector<int> waitTime;
        std::vector<int> turnAroundTime;
        std::vector<int> processId;
        std::vector<int> burstTime;
        std::vector<int> priority;
        std::vector<int> arrivalTime;
        std::vector<int> remainingTime;
        double averageWaitTime;
        double averageTurnAroundTime;
};