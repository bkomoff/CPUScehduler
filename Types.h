#pragma once

namespace PCBTypes
{
    enum ProcessState
    {
        new_process,
        running_process,
        waiting_process,
        ready_process,
        terminated_process
    };  
}
