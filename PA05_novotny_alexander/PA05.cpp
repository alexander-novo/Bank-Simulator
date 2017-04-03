#include "PA05.h"

int main ()
{
    pQueue events;

    loadEvents ( events, EVENT_FILE );
}

void loadEvents ( pQueue& events, const char* filename )
{
    std::ifstream input ( filename );
    Event add;
    add.type = Event::ARRIVAL;

    while ( input.peek () != EOF )
    {
        input >> add.start >> add.duration;
        events.push ( add );
    }
}