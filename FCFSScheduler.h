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

    private:
        ProcessQueue *ready;
        std::vector<int> processId;
        std::vector<int> burstTime;

};