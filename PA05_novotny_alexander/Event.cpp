#include "Event.h"


bool Event::operator<( const Event& e ) const
{
    return start < e.start;
}

bool Event::operator>( const Event& e ) const
{
    return start > e.start;
}