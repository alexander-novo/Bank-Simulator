#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#define EVENT_FILE "events_1.txt"

struct Event;

void loadEvents ( std::vector<Event>& events, const char* filename );

struct Event
{
    enum Type
    {
        ARRIVAL,
        DEPARTURE
    } type;
    unsigned start;
    unsigned duration;
};