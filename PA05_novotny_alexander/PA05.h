// PROGRAM HEADER /////////////////////////////////////////////////////////////
#pragma once

// INCLUDES ///////////////////////////////////////////////////////////////////
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

// CLASSES ////////////////////////////////////////////////////////////////////
#include "Event.h"
#include "PriorityQueue.h"
#include "Queue.h"

//Types to use for simulation
typedef PriorityQueue<Event> pQueue;
typedef Queue<Event> Line;

//Statistics struct for keeping track of simulation statistics
struct Stats
{
    unsigned numTellers;
    unsigned numLines;
    unsigned currentTime;

    unsigned long long totalWaitTime = 0;
    unsigned totalCustomersServed = 0;
    unsigned maximumWait = 0;
    unsigned maximumLength = 0;
};

// PROTOTYPES /////////////////////////////////////////////////////////////////
void loadEvents ( pQueue& events, const char* filename );
void simulate ( pQueue& events, Stats& stats);
void processEvents ( pQueue& events, Line* lines, bool* tellers, Stats& stats );
void processLines ( pQueue& events, Line* lines, bool* tellers, Stats& stats );
Line* findLongestWait ( Line* lines, Stats& stats );
Line* findShortestLine ( Line* lines, Stats& stats );
bool shouldEndSimulation ( pQueue& events, Line* lines, bool* tellers, Stats& stats );
void calculateStats ( const Stats& stats );
