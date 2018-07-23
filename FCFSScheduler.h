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
        double GetAverageWaitTime() const { return averageWaitTime; }
        double GetAverageTurnAroundTime() const { return averageTurnAroundTime; }

    private:
        bool ReadFile( std::string location );
        void PrintOutput();
        void PrintGnattChart( long processId, size_t time );

    private:
        ProcessQueue *ready;
        std::vector<int> waitTime;
        std::vector<int> turnAroundTime;
        std::vector<int> processId;
        std::vector<int> priority;
        std::vector<int> arrivalTime;
        std::vector<int> burstTime;
        double averageWaitTime;
        double averageTurnAroundTime;
};