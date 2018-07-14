#include <vector>

#include "ProcessControlBlock.h"

class ProcessControlBlock;

class ProcessQueueNode
{
    public:
        ProcessQueueNode();
        ~ProcessQueueNode() {};

		void SetData( ProcessControlBlock *pcb ) { data = pcb; }
		ProcessControlBlock *GetData() const { return data; }

		ProcessQueueNode *GetNext() const { return next; }
		void SetNext( ProcessQueueNode *nextNode ) { next = nextNode; }

		ProcessQueueNode *GetPrev() const { return prev; }
		void SetPrev( ProcessQueueNode *prevNode ) { prev = prevNode; }

    private:
		ProcessControlBlock *data;
		ProcessQueueNode    *next;
		ProcessQueueNode    *prev;
};

class ProcessQueue
{
    public:
        ProcessQueue();
		~ProcessQueue();

		void AddProcess( ProcessControlBlock &pcb );


		void AddProcess( long processId );
		void AddProcess( long existingProcessId, long processId );
		void DeleteProcessFromQueue();
		void DeleteProcessFromQueue( long processId );
		void PrintQueue() const;

		unsigned int Size() const { return counter; }  
	
    private:
		ProcessQueueNode *head;
		ProcessQueueNode *tail;
		unsigned int counter;

		ProcessQueueNode *CreateQueueNode( ProcessControlBlock &pcb );

		ProcessQueueNode *CreateQueueNode( long processId );
		bool ComparePIDs( long processsId );
};
