#include "pch.h"
#include "application.h"

void run()
{
    application::init();
    application::run();
}

int main() 
{
    try
    {
        run();
    }
    catch (...)
    {
        LOGERROR("something bad happened");
    }

    return 0;
}
