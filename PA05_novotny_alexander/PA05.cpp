#include "PA05.h"

int main ()
{
    std::vector<Event> events;

    loadEvents ( events, EVENT_FILE );
}

void loadEvents ( std::vector<Event>& events, const char* filename )
{
    std::ifstream input ( filename );
    Event add;
    add.type = Event::ARRIVAL;

    while ( input.peek () != EOF )
    {
        input >> add.start >> add.duration;
        events.push_back ( add );
    }
}