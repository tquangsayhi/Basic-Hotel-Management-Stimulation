#include "visit.hh"
#include <iostream>
#include <fstream>

Visit::Visit(shared_ptr<Room> r , bool active,const Date& d)
{
    // Initialize visit with given room, activity state, and start date
    room_ = r;
    active_ = active;
    start_ = d;
}
Visit::~Visit()
{
    // Destructor (default cleanup)
}
bool Visit::is_active()
{
    return active_;
}
shared_ptr<Room> Visit::return_room()
{
    return room_;
}
void Visit::end_visit(const Date& d)
{
    // Mark the visit as ended and set the end date
    active_ = false;
    end_ = d;
    // Free up one bed in the room
    room_->available_slot += 1;
     // If any slot becomes free, mark the room as not fully occupied
    if (room_->available_slot > 0)
    {
       room_->occupied = false;
    }
}
void Visit::display_time()
{
    // Print the duration of the visit
    start_.print();
    cout << " - ";
    if (!active_)
    {
        end_.print();
    }
}
