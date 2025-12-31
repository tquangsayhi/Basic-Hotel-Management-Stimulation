
/* Class Visit
 * ----------
 * COMP.CS.110 FALL 2025
 * ----------
 * Class for describing a guest's visit in hotel.
 *
 * Note: Students need to implement almost all of this class by themselves.
 * */
#ifndef VISIT_HH
#define VISIT_HH
#include "date.hh"
using namespace std;
#include <vector>
#include<memory>

// ==========================
// Struct: Room
// Represents a single hotel room.
// ==========================
struct Room
{
    unsigned int id;         // unique ID room
    unsigned int size;       // room size
    unsigned available_slot; //available space left
    bool occupied;           // true if currently fully booked
};

// ==========================
// Struct: Person
// Represents a hotel guest.
// ==========================
struct Person
{
    string name;   //guest name
    //Date DOB;    // (optional future extension)
};



class Visit
{
public:
    // TODO: Add parameters for the constructor if needed
    /**
     * @brief Constructor for Visit
     * @param r pointer to the room assigned to the guest
     * @param active true if the visit is ongoing
     * @param d start date of the visit
     */
    Visit(shared_ptr<Room> r , bool active, const Date& d);
    ~Visit();
    // TODO: More public methods

    /**
     * @brief is_active
     * @return true if the visit is currently active.
     */
    bool  is_active();

    /**
     * @brief return_room
     * @return pointer to the room associated with this visit.
     */
    shared_ptr<Room> return_room();

    /**
    * @brief end_visit
    * Marks the visit as ended, sets the end date,
    * and frees up one slot in the associated room.
    * @param d the date when the visit ends.
    */
    void end_visit(const Date& d);

    /**
     * @brief display_time
     * Prints the start and end date of the visit.
     * If still active, only the start date is shown.
     */
    void display_time();
private:
    Date start_;
    Date end_;
    // TODO: More attributes and private methods
    shared_ptr<Room> room_;
    bool active_;
};
#endif // VISIT_HH
