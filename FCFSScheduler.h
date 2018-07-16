#pragma once

class ProcessQueue;

class FCFSScheduler
{
    public:
        FCFSScheduler();
        ~FCFSScheduler();

        bool Initialize( std::string location );
        void Execute();

    private:
        ProcessQueue *ready;
};