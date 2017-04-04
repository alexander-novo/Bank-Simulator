#include "Event.h"


bool Event::operator<( const Event& e ) const
{
    if ( start == e.start )
    {
        return duration < e.duration;
    }
    else
    {
        return start < e.start;
    }
}

bool Event::operator>( const Event& e ) const
{
    if ( start == e.start )
    {
        return duration > e.duration;
    }
    else
    {
        return start > e.start;
    }
}