#pragma once

#include <vector>

class ProcessQueue;

class FCFSScheduler
{
    public:
        FCFSScheduler();
        ~FCFSScheduler();

        bool Initialize( std::string location );
        void Execute();

    private:
        bool ReadFile( std::string location );
        void PrintOutput();

    private:
        ProcessQueue *ready;
        std::vector<int> waitTime;
        std::vector<int> turnAroundTime;
        std::vector<int> processId;
        std::vector<int> priority;
        std::vector<int> burstTime;

};