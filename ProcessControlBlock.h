#pragma once

#include "Types.h"

using namespace PCBTypes;

class ProcessControlBlock
{
    public:
        ProcessControlBlock( long pid ):
			currentState(PCBTypes::new_process),
			processId(pid),
			arrivalTime(0),
			burstTime(0),
			priorty(0)
		{}

        ~ProcessControlBlock() {};

	enum ProcessState GetProcessState() const { return currentState; };
	void SetProcessState( enum ProcessState state ) { currentState = state; }
	long GetProcessId() const { return processId; };
	void SetProcessId( long id ) { processId = id; }
	long GetArrivalTime() const { return arrivalTime; }
	void SetArrivalTime( long time ) { arrivalTime = time; }
	long GetBurstTime() const { return burstTime; }
	void SetBurstTime( long time ) { burstTime = time; }
	long GetPriorty() const { return priorty; }
	void SetPriorty( long pri ) { priorty = pri; }

    private:
	enum ProcessState currentState;
	long processId;
	long arrivalTime;
	long burstTime;
	long priorty;
};
