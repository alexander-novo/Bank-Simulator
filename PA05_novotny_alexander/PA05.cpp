#include "PA05.h"

int main ( int argc, char* argv[] )
{
    pQueue events;
    Stats stats;

    if ( argc < 3 )
    {
        std::cout << "Not enough arguments were found" << std::endl
            << "Usage - banksim <inputFile> <# of tellers> <# of lines>" << std::endl
            << "Consider trying test.sh" << std::endl;

        return 1;
    }

    loadEvents ( events, argv [ 1 ] );

    stats.numTellers = atoi ( argv [ 2 ] );
    stats.numLines = atoi ( argv [ 3 ] );

    simulate ( events, stats );
}

void loadEvents ( pQueue& events, const char* filename )
{
    std::ifstream input ( filename );
    Event add;
    add.type = Event::ARRIVAL;

    while ( input.peek () != EOF )
    {
        input >> add.start >> add.duration;
        if ( input.eof() ) break;
        events.push ( add );
    }
}

void simulate ( pQueue& events, Stats& stats )
{
    Line* lines = new Line [ stats.numLines ];
    bool* tellers = new bool [ stats.numTellers ];
    bool end = false;

    stats.currentTime = 0;

    //Initialise tellers to false
    std::memset ( tellers, false, stats.numTellers );

    while ( !end )
    {
        //Then check our events
        processEvents ( events, lines, tellers, stats );

        //Process every line
        processLines ( events, lines, tellers, stats );

        end = shouldEndSimulation ( events, lines, tellers, stats );

        stats.currentTime++;
    }

    calculateStats ( stats );

    delete[] lines;
    delete[] tellers;
}

void processEvents ( pQueue& events, Line* lines, bool* tellers, Stats& stats )
{
    //Keep processing the top event as long as it should be happening right now
    while ( events.size () && events.top ().start == stats.currentTime )
    {
        Event currentEvent = events.top ();
        switch ( currentEvent.type )
        {
            case Event::ARRIVAL:
                //Find shortest line and add person to it
                findShortestLine ( lines, stats )->push ( currentEvent );
                std::cout << "Added to line at " << stats.currentTime << std::endl;
                break;
            case Event::DEPARTURE:
                //Free up the teller
                tellers [ currentEvent.teller ] = false;
                //Another happy customer
                stats.totalCustomersServed++;

                std::cout << "Freed up teller " << currentEvent.teller
                    << " at " << stats.currentTime << std::endl;
                break;
        }
        //After processing an event, move on
        events.pop ();
    }
}

void processLines ( pQueue& events, Line* lines, bool* tellers, Stats& stats )
{
    //Find an open teller
    for ( unsigned tellerIndex = 0; tellerIndex < stats.numTellers; tellerIndex++ )
    {
        //Find an empty teller
        if ( !tellers [ tellerIndex ] )
        {
            Line* comeFrom = findLongestWait ( lines, stats );
            if ( comeFrom != nullptr )
            {
                //Reserve the teller
                tellers [ tellerIndex ] = true;

                //Create our departure event
                Event departEvent;
                departEvent.type = Event::DEPARTURE;
                departEvent.start = stats.currentTime + comeFrom->front ().duration;
                departEvent.teller = tellerIndex;

                //Add it to the event queue
                events.push ( departEvent );

                //Check if the person that just went up has waited the longest
                if ( ( stats.currentTime - comeFrom->front ().start ) > stats.maximumWait )
                {
                    stats.maximumWait = stats.currentTime - comeFrom->front ().start;
                }

                //And move the line along
                comeFrom->pop ();

                std::cout << "Reserved teller " << tellerIndex << " at "
                    << stats.currentTime << " until " << departEvent.start << std::endl;
            }
        }
    }

    //Keep track of the people waiting in line
    for ( unsigned lineIndex = 0; lineIndex < stats.numLines; lineIndex++ )
    {
        stats.totalWaitTime += lines [ lineIndex ].size ();
        if ( lines [ lineIndex ].size () > stats.maximumLength )
        {
            stats.maximumLength = lines [ lineIndex ].size ();
        }
    }
}

Line* findLongestWait ( Line* lines, Stats& stats )
{
    Line* min = nullptr;

    //Find our smallest arrival time - be careful, lines might be empty
    for ( unsigned lineIndex = 0; lineIndex < stats.numLines; lineIndex++ )
    {
        if ( lines [ lineIndex ].size () == 0 ) continue;
        //If min is uninitialised or our index is less than min, we have a new min
        if ( min == nullptr || lines [ lineIndex ].front () < min->front () )
        {
            min = lines + lineIndex;
        }

    }

    return min;
}

Line* findShortestLine ( Line* lines, Stats& stats )
{
    //Assume shortest is the first one
    Line* min = lines;

    //Find shortest line by comparing to min
    for ( unsigned lineIndex = 1; lineIndex < stats.numLines; lineIndex++ )
    {
        if ( lines [ lineIndex ].size () < min->size () )
        {
            min = lines + lineIndex;
        }
    }

    return min;
}

bool shouldEndSimulation ( pQueue& events, Line* lines, bool* tellers, Stats& stats )
{
    //If we have any events left, don't end
    if ( !events.empty () ) return false;
    //If any of our tellers are occupied, don't end
    for ( unsigned tellerIndex = 0; tellerIndex < stats.numTellers; tellerIndex++ )
    {
        if ( tellers [ tellerIndex ] ) return false;
    }

    return true;
}

void calculateStats ( const Stats& stats )
{
    std::cout << std::endl << "\tFinal Statistics:" << std::endl
        << "Number of tellers: " << stats.numTellers << std::endl
        << "Number of lines: " << stats.numLines << std::endl
        << "Total number of customers served: " << stats.totalCustomersServed << std::endl
        << "Average wait time: " << ( stats.totalWaitTime / ( double ) stats.totalCustomersServed ) << std::endl
        << "Maximum wait time: " << stats.maximumWait << std::endl
        << "Average line length: " << ( stats.totalWaitTime / ( double ) stats.currentTime / stats.numLines ) << std::endl
        << "Maximum line length: " << stats.maximumLength << std::endl;

    std::cout << std::endl;
}
