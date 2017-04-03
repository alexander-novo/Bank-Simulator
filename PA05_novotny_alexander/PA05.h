#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

#include "Event.h"

#define EVENT_FILE "events_1.txt"

typedef std::priority_queue<Event, std::vector<Event>, std::greater<Event>> pQueue;

void loadEvents ( pQueue& events, const char* filename );