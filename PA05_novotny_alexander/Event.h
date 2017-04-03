#pragma once

class Event
{
    public:
        enum Type
        {
            ARRIVAL,
            DEPARTURE
        } type;
        unsigned start;
        unsigned duration;

        bool operator>( const Event& ) const;
};