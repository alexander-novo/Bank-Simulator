//  PROGRAM INFORMATION ///////////////////////////////////////////////////////
/**
  * @file PA05.cpp
  *
  * @brief Simulator to test bank queues
  *
  * @details Simulator which takes in a list of arrival events consisting of
  *          an arrival time and duration and simulates how long people would
  *          have to wait in a line for the bank teller
  *
  * @version 1.00
  *          Alexander Novotny
  *          First Version
  *
  * @notes Requires command line arguments. 
  *        <command> <filename> <# tellers> <# lines>
  *        filename - name of the file to load arrival events from
  *        # tellers - number of bank tellers to use in the simulation
  *        # lines - number of queues for people to wait in in the simulation
  */

// MAIN HEADER ////////////////////////////////////////////////////////////////
#include "PA05.h"

// MAIN FUNCTION //////////////////////////////////////////////////////////////
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

/**
  * @brief Loads a list of arrival events from a file
  *
  * @param[in] filename
  *            The name of the file to load from
  *
  * @param[out] events
  *             A priority queue to hold the events
  *
  * @post events
  *       Events will be filled with the loaded events
  */
void loadEvents ( pQueue& events, const char* filename )
{
    std::ifstream input ( filename );
    Event add;
    add.type = Event::ARRIVAL;

    while ( input.peek () != EOF )
    {
        input >> add.start >> add.duration;
        //For linux - input.peek() doesn't work, maybe because of line endings?
        if ( input.eof() ) break;
        events.push ( add );
    }
}
/**
  * @brief Main simulation function
  *
  * @details Runs an entire simulation on the events given as described by stats
  *          and keeps track of certain statistics in stats
  *
  * @param[in] events
  *            The queue of events to simulate on
  *
  * @param[in] stats
  *            stats.numLines and stats.numTellers will specify the parameters
  *            of the simulation
  *
  * @param[out] stats
  *             Contains the statistics of the finished simulation
  *
  * @pre stats
  *      Must contain a valid numLines and numTellers
  *
  * @post events
  *       Will be empty
  *
  * @post stats
  *       Will contain statistics of the finished simulation
  *
  * @par Algorithm
  *      Every unit of time, checks if a new arrival or departure event has occured,
  *      adds it to the shortest line, and then moves the lines up once someone
  *      has departed from the teller
  *
  * @note See processEvents(), processLines(), shouldEndSimulation()
  */
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
        //Check if there are any new events occurring
        processEvents ( events, lines, tellers, stats );

        //Move the lines up if someone has left the teller
        processLines ( events, lines, tellers, stats );

        //Based on what has happened, should we end yet?
        end = shouldEndSimulation ( events, lines, tellers, stats );

        stats.currentTime++;
    }

    calculateStats ( stats );

    delete[] lines;
    delete[] tellers;
}

/**
  * @brief Checks if any new events have occured, and processes them
  *
  * @param[in] events
  *            The queue of events to check
  *
  * @param[in] lines
  *            An array of queues which hold people waiting for a teller
  *
  * @param[in] tellers
  *            An array of booleans signifying a person waiting at a teller
  *
  * @param[in] stats
  *            Number of tellers and lines, as well as current time
  *
  * @post events
  *       Might have decreased in size if events have been processed
  *
  * @post lines
  *       Might have more people in them if an arrival event was processed
  *
  * @post tellers
  *       Some tellers might have been freed up if a departure event was processed
  */
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

/**
  * @brief Moves people from the from the front of a line into an empty teller
  *
  * @param[in] lines
  *            An array of queues holding waiting people
  *
  * @param[in] tellers
  *            An array of booleans which represent busy tellers
  *
  * @param[in] stats
  *            Keeps track of number of lines and tellers, as well as current time
  *      
  * @param[out] events
  *            A priority queue of events for adding departure events
  *
  * @post events
  *       New departure events will have been added if someone has started seeing a teller
  *
  * @post lines
  *       Some people might have exited their line to go to a teller
  *
  * @post tellers
  *       Unocuppied tellers will become occupied if someone is waiting in a line
  */
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


/**
  * @brief Finds the line with the person in front who has been waiting the longest
  *
  * @param[in] lines
  *            An array of lines to check through
  *
  * @param[in] stats
  *            The number of lines
  *
  * @return Pointer to the line with the longest waiting person in front
  */
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

/**
  * @brief Finds the line with the fiewest people waiting in it
  *
  * @param[in] lines
  *            The list of lines to search through
  *
  * @param[in] stats
  *            The number of lines
  *
  * @return Pointer to the shortest line
  */
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

/**
  * @brief Determines whether the simulation should end
  *
  * @param[in] events
  *            A list of remaining events
  *
  * @param[in] lines
  *            A list of lines which contain waiting people
  *
  * @param[in] tellers
  *            A list of booleans which determine if a teller is busy
  *
  * @param[in] stats
  *            Number of tellers
  *
  * @par Algorithm
  *      If there are any events left, we haven't finished yet
  *
  * @return Whether the simulation should end
  */
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

/**
  * @brief Calculates and prints stats about a simulation
  *
  * @param[in] stats
  *            The stats object passed to the simulation used to keep track of 
  *            the simulation statistics
  */
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
