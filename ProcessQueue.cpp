#include <iostream>

#include "ProcessControlBlock.h"
#include "ProcessQueue.h"

ProcessQueueNode::ProcessQueueNode():
    data( nullptr ),
    next( nullptr ),
    prev( nullptr )
{}

ProcessQueue::ProcessQueue():
    head( nullptr ),
    tail( nullptr ),
    counter( 0 )
{}

ProcessQueue::~ProcessQueue()
{  
}

ProcessQueueNode *ProcessQueue::CreateQueueNode( ProcessControlBlock *pcb )
{
    ProcessQueueNode *node = new ProcessQueueNode;  
    if ( node != nullptr )
    {
        node->SetData( pcb );
        node->SetNext( nullptr );
        node->SetPrev( nullptr );
    }

    return node;    
}

bool ProcessQueue::ComparePIDs( long processId )
{
    bool found = false;

    int i = 0;
    ProcessQueueNode *currentNode = head;
    while ( i < counter && !found)
    {
        found = currentNode->GetData()->GetProcessId() == processId;
        if ( !found )
        {
            currentNode = currentNode->GetNext(); 
            i++;
        }
    }

    return found;
}

void ProcessQueue::AddProcess( ProcessControlBlock *pcb )
{
    //Check to see if PID exists
    bool exist = ComparePIDs( pcb->GetProcessId() );

    if ( !exist )
    {
        ProcessQueueNode *newNode = CreateQueueNode( pcb );
        if ( head == nullptr ) // empty list
        {
            this->head = newNode;
        }
        else if ( tail == nullptr ) // contains one item
        {
            this->tail = newNode;
            newNode->SetPrev( this->head );
            this->head->SetNext( this->tail );
        }
        else // more than one item in list add to end
        {
            newNode->SetPrev( this->tail );
            this->tail->SetNext( newNode );
            this->tail = newNode;
        }
        
        counter++;
    }
    else
    {
        std::cout << pcb->GetProcessId() << " already exist" << std::endl;
    }
}

void ProcessQueue::AddProcess( ProcessControlBlock *pcb, long existingProcessId )
{
    if ( head == nullptr )
    {
        std::cout << "Empty list" << std::endl;
        return;
    }

    //Check to see if PID exists
    bool exist = ComparePIDs( pcb->GetProcessId() );

    if ( !exist )
    {
        ProcessQueueNode *currentNode = this->head;
        bool found = false;
        for ( int i = 0; i < counter && !found; i++ )
        {
            if ( currentNode->GetData()->GetProcessId() != existingProcessId )
            {
                currentNode = currentNode->GetNext();
            }
            else
            {
                ProcessQueueNode *newNode = CreateQueueNode( pcb );
                if ( currentNode->GetNext() == nullptr )
                {
                    this->tail = newNode;
                    this->tail->SetPrev( currentNode );
                    currentNode->SetNext( newNode );
                }
                else
                {
                    newNode->SetPrev( currentNode );
                    newNode->SetNext( currentNode->GetNext() );
                    currentNode->SetNext( newNode );
                    newNode->GetNext()->SetPrev( newNode );
                }

                counter++;
                found = true;
            }
        }
    }
    else
    {
        std::cout << pcb->GetProcessId() << " already exist" << std::endl;
    }    
}

void ProcessQueue::DeleteProcessFromQueue()
{
    if ( head == nullptr )
    {
        std::cout << "Nothing to delete" << std::endl;
        return;
    }
 
    ProcessQueueNode *currentNode = head;
    if ( currentNode != nullptr )
    {
        this->head = nullptr;
        this->head = currentNode->GetNext();
        if ( this->head != nullptr )
        {
            this->head->SetPrev(nullptr);
        }
        delete currentNode;
        counter--;
    }
}

void ProcessQueue::DeleteProcessFromQueue( long processId )
{   
    ProcessQueueNode *currentNode = head;
    int i = 0;
    bool done = false;
    while ( i < counter && !done )
    {
        if ( currentNode->GetData()->GetProcessId() == processId )
        {
            if ( currentNode->GetPrev() == nullptr )
            {
                this->head = nullptr;
                this->head = currentNode->GetNext();
                if ( this->head != nullptr )
                {
                    this->head->SetPrev(nullptr);
                }
            }
            else if ( currentNode->GetNext() == nullptr )
            {
                this->tail = nullptr;
                this->tail = currentNode->GetPrev();
                if ( this->tail != nullptr )
                {
                    this->tail->SetNext(nullptr);
                }
            }
            else
            {
                ProcessQueueNode *next = currentNode->GetNext();
                ProcessQueueNode *prev = currentNode->GetPrev();
                next->SetPrev(prev);
                prev->SetNext(next);
            }
            delete currentNode;
            currentNode = nullptr;
            counter--;
            done = true;
        }
        else
        {
            currentNode = currentNode->GetNext();
            i++;
        }
    }

    if ( !done )
    {
        std::cout << "PID does not exist" << std::endl;
    }
}

void ProcessQueue::PrintQueue() const
{
    int i = 0;
    ProcessQueueNode *currentNode = head;
    std::cout << "Queue list: ";
    while ( i < counter )
    {
        std::cout << currentNode->GetData()->GetProcessId() << " ";
        currentNode = currentNode->GetNext(); 
        i++;
    }
    std::cout << std::endl;
}

ProcessControlBlock const *ProcessQueue::Get( size_t index )
{
    ProcessControlBlock const *pcb = nullptr;

    ProcessQueueNode const *currentNode = this->GetHead();
    for ( size_t i = 0; i < Size(); i++ )
    {
        if ( i == index )
        {
            pcb = currentNode->GetData();
            break;
        }

        currentNode = currentNode->GetNext();
    }
    
    return pcb;
}