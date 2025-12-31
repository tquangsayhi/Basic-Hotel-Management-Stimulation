/* Hotel program
 * -------------
 * COMP.CS.110 FALL 2025
 * -------------
 * This program simulates the operation of a small hotel. It reads
 * initial room information from an input file given by the user, where
 * each line describes a single room in the form:
 *
 * room_number;capacity
 *
 * The program stores this information into appropriate data structures
 * based on STL containers and structs (such as maps, vectors, and shared
 * pointers). After initialization, the user can control the hotel
 * through a command-line interface by giving various commands.
 *
 * Available commands include setting and advancing the current date,
 * booking rooms for guests, marking guests as leaving, and printing
 * information about rooms, guests, and visits. The program keeps track
 * of both current and past visits, allowing the user to view statistics
 * such as the most frequent (honor) guests.
 *
 * Each command and its parameters are validated carefully, and possible
 * errors (such as invalid room numbers or double bookings) are handled
 * gracefully. The program prints informative messages to guide the user.
 *
 * The program terminates when the user gives the command "QUIT".
 *
 * Author 1:
 * Le Nguyen Tuong Quang - quang.le@tuni.fi - 153126132
 * Author 2:
 * Sai Cong Minh - minh.sai@tuni.fi - 153126022
 *
 */

#include "cli.hh"
#include "hotel.hh"
#include <string>
#include <memory>

using namespace std;

const string PROMPT = "Hotel> ";

int main()
{
    shared_ptr<Hotel> hotel = make_shared<Hotel>();
    if ( not hotel->init() )
    {
        return EXIT_FAILURE;
    }

    Cli cli(hotel, PROMPT);
    while ( cli.exec() );

    return EXIT_SUCCESS;
}
