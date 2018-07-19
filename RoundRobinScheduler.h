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

    private:
        bool ReadFile( std::string location );
        void MoveProcess( ProcessControlBlock &pcb, size_t time );

    private:
        const size_t timeQuantum;
        ProcessQueue *ready;
        std::vector<int> processId;
        std::vector<int> burstTime;
};