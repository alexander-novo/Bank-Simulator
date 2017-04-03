#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
#include <cstring>

#include "Event.h"

#define EVENT_FILE "events_1.txt"

typedef std::priority_queue<Event, std::vector<Event>, std::greater<Event>> pQueue;
typedef std::queue<Event> Line;

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

void loadEvents ( pQueue& events, const char* filename );
void simulate ( pQueue& events, Stats& stats);
void processEvents ( pQueue& events, Line* lines, bool* tellers, Stats& stats );
void processLines ( pQueue& events, Line* lines, bool* tellers, Stats& stats );
Line* findLongestWait ( Line* lines, Stats& stats );
Line* findShortestLine ( Line* lines, Stats& stats );
bool shouldEndSimulation ( pQueue& events, Line* lines, bool* tellers, Stats& stats );
void calculateStats ( const Stats& stats );
