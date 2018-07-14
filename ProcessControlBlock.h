#include "Types.h"

using namespace PCBTypes;

class ProcessControlBlock
{
    public:
        ProcessControlBlock( long pid );
        ~ProcessControlBlock();

	enum ProcessState GetProcessState() const { return currentState; };
	void SetProcessState( enum ProcessState state ) { currentState = state; }
	long GetProcessId() const { return processId; };
	void SetProcessId( long id ) { processId = id; }

    private:
	enum ProcessState currentState;
	long processId;
};
