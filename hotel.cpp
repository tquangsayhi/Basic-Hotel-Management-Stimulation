#include "hotel.hh"
#include "utils.hh"
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>

// Error and information outputs
const string FILE_NOT_FOUND = "Error: Input file not found."s;
const string WRONG_FORMAT = "Error: Wrong format in file."s;
const string NOT_NUMERIC = "Error: Wrong type of parameters."s;
const string ALREADY_EXISTS = "Error: Already exists: "s;
const string CANT_FIND = "Error: Can't find anything matching: "s;
const string GUEST_ENTERED = "A new guest has entered."s;
const string GUEST_LEFT = "Guest left hotel, visit closed."s;
const string FULL = "Error: Can't book, no such rooms available."s;

Hotel::Hotel()
{
    //cout << "Hotel constructor" << endl;
}

Hotel::~Hotel()
{
    //cout << "Hotel destructor" << endl;
}

bool Hotel::init()
{
    cout << "Input file: ";
    string file_name = "";
    getline(cin, file_name);
    ifstream file(file_name);
    if ( not file )
    {
        cout << FILE_NOT_FOUND << endl;
        return false;
    }

    string line = "";
    unsigned int room_id = 1;
    while ( getline(file, line) )
    {
        vector<string> parts = utils::split(line, ';');
        if ( parts.size() != 2 )
        {
            cout << WRONG_FORMAT << endl;
            return false;
        }
        if ( not ( utils::is_numeric(parts.at(0), false) and
                   utils::is_numeric(parts.at(1), false) ) )
        {
            cout << NOT_NUMERIC << endl;
            return false;
        }

        unsigned int amount = stoi(parts.at(1));
        for ( unsigned int i = 0; i < amount; ++i )
        {
            unsigned int size = stoi(parts.at(0));
            // TODO: Write code that creates rooms and adds them into a suitable data structure
            // Create and store room objects to the data structures
            shared_ptr<Room> room = make_shared<Room>(Room{room_id, size, size, false});
            rooms_[room_id] = room;
            room_id ++;
        }
    }
    return true;
}

void Hotel::set_date(Params params)
{
    string day = params.at(0);
    string month = params.at(1);
    string year = params.at(2);
    if ( not utils::is_numeric(day, false) or
         not utils::is_numeric(month, false) or
         not utils::is_numeric(year, false) )
    {
        cout << NOT_NUMERIC << endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    cout << "Date has been set to ";
    utils::today.print();
    cout << endl;
}

void Hotel::advance_date(Params params)
{
    string amount = params.at(0);
    if ( not utils::is_numeric(amount, true) )
    {
        cout << NOT_NUMERIC << endl;
        return;
    }
    utils::today.advance(stoi(amount));
    cout << "New date is ";
    utils::today.print();
    cout << endl;
}

shared_ptr<Room> Hotel::find_free_room(unsigned int size)
{
    // Search for a room with matching size that is not fully occupied
    shared_ptr<Room> selected_room = nullptr;
    for (const auto& [id, room] : rooms_)   // iterate over the rooms_ map
    {
        // Room must match size and not be fully occupied
        if (room->size == size && !room->occupied )
        {
            // Return immediately if the room is completely free
            if (room->available_slot == room->size)
            {
                return room;
            }
            // Otherwise choose the room with more available slots
            if (!selected_room)
            {
                selected_room = room;
            } else {
                if (room->available_slot > selected_room->available_slot)
                {
                    selected_room = room;
                }
            }
        }
    }
    return selected_room;
}

void Hotel::print_visits_for(const string& guest_name)
{
    // Print all visits (past & present) of a guest
    for (const auto& visit : visits_[guest_name])
    {
        cout << "* Visit: ";
        visit->display_time();
        cout << endl;
    }
}

void Hotel::print_rooms(Params /*params*/)
{
    // Print each room and its occupancy status
        for (const auto& [id,room]: rooms_)
        {
            if (room->available_slot != 0)
            {
                cout << "Room " << room->id
                << " : for " << room->size << " person(s) : available for "
                << room->available_slot << " person(s)" << endl;
            }
            else {
                cout << "Room " << room->id
                << " : for " << room->size << " person(s) : full" << endl;
            }
        }
}

void Hotel::book(Params params)
{
    // Extract parameters
    string guest_name = params.at(0);
    string room_size_str = params.at(1);
    // Validate room size
    if (!utils::is_numeric(room_size_str, false))
    {
        cout << NOT_NUMERIC << endl;
        return;
    }
    unsigned int requested_size = stoi(room_size_str);

    // Find a suitable room
    auto room = find_free_room(requested_size);
    if(!room) {
        cout << FULL << endl;
        return;
    }

    shared_ptr<Person> person;
    // Determine if guest already exists
    if (people_.find(guest_name) != people_.end())
    {
        person = people_[guest_name];
        // Prevent double-booking an active visit
        if (!visits_[guest_name].empty() && visits_[guest_name].back()->is_active())
        {
            cout << ALREADY_EXISTS << guest_name << endl;
            return;
        }
    } else {
        // Create a new guest entry
        person = make_shared<Person>(Person{guest_name});
        people_[guest_name] = person;
        visits_[guest_name] = {};
    }

    // Update room occupancy
    room->available_slot -= 1;
    if (room->available_slot == 0)
        room->occupied = true;

     // Create and register a new visit
    shared_ptr<Visit> visit = make_shared<Visit>(Visit(room,true, utils::today));
    visits_[guest_name].push_back(visit);
    cout << GUEST_ENTERED << endl;
}

void Hotel::leave(Params params)
{
    string guest_name = params.at(0);
    shared_ptr<Visit> visit = nullptr;

    // Guest not found
    if (people_.find(guest_name) == people_.end())
    {
        cout << CANT_FIND << guest_name << endl;
        return;
    }
    else {
        visit = visits_[guest_name].back();
        // Guest already inactive
        if (!visit->is_active())
        {
            cout << CANT_FIND << guest_name << endl;
            return;
        }
    }
    // Mark visit as ended and update room availability
    visit->end_visit(utils::today);

    cout << GUEST_LEFT << endl;
}

void Hotel::print_guest_info(Params params)
{
    // Print all visits for a single guest
    string guest_name = params.at(0);
    if (people_.find(guest_name) == people_.end())
    {
        cout << CANT_FIND << guest_name << endl;
        return;
    }
    print_visits_for(guest_name);
}

void Hotel::print_all_visits(Params /*params*/)
{
    //Return immediately if the data is empty
    if (visits_.empty())
    {
        cout << "None" << endl;
        return;
    }
     // Print every guest with their visit history
    for (const auto& [person, visits] : visits_)
    {
        cout << person<< endl;
        print_visits_for(person);
    }
}

void Hotel::print_current_visits(Params /*params*/)
{
    //Return immediately if the data is empty
    if (visits_.empty())
    {
        cout << "None" << endl;
        return;
    }
    //initialise a bool to check if there is any guest at the moment
    bool currently_have_guest = false;
    shared_ptr<Visit> visit = nullptr;
    //The idea is that we go through all of the guests and check their last visit.
    for (const auto& [person,visits] : visits_)
    {
        if (visits.empty()) continue;
        visit = visits.back();
        if (visit && visit->is_active())
        {
            cout << person << " is boarded in Room " << visit->return_room()->id << endl;
            currently_have_guest = true;
        }
    }
    if (!currently_have_guest)
    {
        cout << "None" << endl;
        return;
    }
}

void Hotel::print_honor_guests(Params /*params*/)
{
    if (visits_.empty())
    {
        cout << "None" << endl;
        return;
    }
    // Track the maximum number of visits among all guests
    unsigned int max_visits = 0;

    // Store the names of guests with that many visits
    vector<string> top_guests;

    unsigned int visit_count;
    //go through the loop to seek for the guests with most boooking
    for (const auto& [person, visits] : visits_)
    {
        visit_count = visits.size();
        if (visit_count > max_visits)
        {
            max_visits = visit_count;
            top_guests.clear();
            top_guests.push_back(person);
        }
        else if (visit_count == max_visits)
        {
            top_guests.push_back(person);
        }
    }
    if (max_visits == 0)
    {
        cout << "None" << endl;
        return;
    }
    // Sort guests alphabetically
    sort(top_guests.begin(), top_guests.end());

    // Print result
    cout << "With " << max_visits << " visit(s), the following guest(s) get(s) honorary award:" << endl;
    for (const auto& name : top_guests)
    {
        cout << "* " << name << endl;
    }
}
