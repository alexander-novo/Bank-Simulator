#include "numGenerator.h"

int main ( int argc, char* argv[] ) {
    if ( argc < 4 )
    {
        std::cout << "Not enough arguments were found" << std::endl
            << "\tUsage - numGenerator <outputFile> <# of events> <max start time> <max duration>" << std:: endl;
        return 1;
    }

    srand ( time ( NULL ) );

    char* filename = argv[ 1 ];
    unsigned numEvents = atoi( argv [ 2 ] );
    unsigned maxStart = atoi( argv [ 3 ] );
    unsigned maxDuration = atoi( argv [ 4 ] );

    Event* events = new Event [ numEvents ];
    
    for ( unsigned currentEvent = 0; currentEvent < numEvents; currentEvent++ )
    {
        Event temp;
        temp.start = rand () % ( maxStart - 1 ) + 1;
        temp.duration = rand () % ( maxDuration - 1 ) + 1;

        unsigned placeFinder;
        //First, find an event that belongs after the new event
        for ( placeFinder = 0; placeFinder < currentEvent; placeFinder++ )
        {
            if ( temp.start > events [ placeFinder ].start )
            {
                //Then shift all of the events to the right
                for ( unsigned shiftIndex = currentEvent - 1; shiftIndex >= placeFinder; shiftIndex-- )
                {
                    events [ shiftIndex + 1 ] = events [ shiftIndex ];
                    if ( !shiftIndex ) break;
                }
                break;
            }
        }
        
        //Place our new event in the hole
        events [ placeFinder ] = temp;
    }

    std::ofstream output ( filename );

    for ( unsigned eventIndex = numEvents - 1; eventIndex >= 0; eventIndex-- )
    {
        output << events [ eventIndex ].start << " " << events [ eventIndex ].duration << std::endl;
        if ( !eventIndex ) break;
    } 

    delete[] events;
}
