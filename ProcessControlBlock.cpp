#include <iostream>

#include "ProcessControlBlock.h"

ProcessControlBlock::ProcessControlBlock( long pid )
{
    currentState = ProcessState::new_process;
    processId    = pid;
}

ProcessControlBlock::~ProcessControlBlock()
{}
