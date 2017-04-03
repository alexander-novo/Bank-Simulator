#pragma once

class Event
{
    public:
        enum Type
        {
            ARRIVAL,
            DEPARTURE
        } type;
        //Keeps track of when the event should happen, be it arrive or depart
        unsigned start;
        //Keeps track of how long the event will happen (if arrival)
        unsigned duration;
        //Keeps track of which teller to depart from (if departure)
        unsigned teller;

        bool operator<( const Event& ) const;
        bool operator>( const Event& ) const;
};