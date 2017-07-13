#include "threadable.h"

Threadable::Threadable()
{
    stopped = true;
    actuallyStopped = true;
}

void Threadable::onStop()
{
    stopped = true;
}
