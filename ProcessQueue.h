#pragma once

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

		void AddProcess( ProcessControlBlock *pcb );
		void AddProcess( ProcessControlBlock *pcb, long existingProcessId );

		void DeleteProcessFromQueue();
		void DeleteProcessFromQueue( long processId );

		void PrintQueue() const;

		ProcessQueueNode const *GetHead() { return head; }

		unsigned int Size() const { return counter; }  
	
    private:
		ProcessQueueNode *head;
		ProcessQueueNode *tail;
		unsigned int counter;

		ProcessQueueNode *CreateQueueNode( ProcessControlBlock *pcb );

		bool ComparePIDs( long processsId );
};
