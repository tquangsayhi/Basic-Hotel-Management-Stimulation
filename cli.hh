/* Class Cli
 * ----------
 * COMP.CS.110 FALL 2025
 * ----------
 * Class for describing a command line interpreter. Preserves all commands
 * used in hotel program. Implements the exec method that calls an
 * appropriate method based on user-given command through a function pointer.
 *
 * Note: Students shouldn't need to make changes to this class.
 * */
#ifndef CLI_HH
#define CLI_HH

#include "hotel.hh"
#include <string>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

/**
 * Declares a type of function called.
 */
using MemberFunc = void (Hotel::*)(const std::vector<std::string>& params);

/**
 * @brief The Cmd struct 'defines' a command function.
 */
struct Cmd
{
    vector<string> aliases;
    string name;
    vector<string> params;
    MemberFunc func_ptr;
};

class Cli
{
public:
    /**
     * @brief Cli
     * @param hotel pointer to the hotel obj the functions are called to
     * @param prompt that is printed before taking in user input
     */
    Cli(shared_ptr<Hotel> hotel, const string& prompt);

    /**
     * @brief exec the cli
     * @return false if execution should end, true if it should continue.
     */
    bool exec();

private:
    /**
     * @brief pop_front
     * @param vec
     * vector utility func that erases the first element of given vector.
     *
     * @note All iterators to the container are invalitaded
     */
    void pop_front(vector<string>& vec);

    /**
     * @brief find_command
     * @param cmd
     * @return Cmd pointer if one with given alias is found.
     */
    Cmd* find_command(const string& cmd);

    /**
     * @brief print_help
     * @param params
     * Generate and print a help, either generic ( no params ) or
     * specific ( func name as param )
     */
    void print_help(const vector<string>& params);

    /**
     * @brief print_cmd_info
     * @param cmd
     * @param longer
     * Print info of a single cmd.
     * if longer == true, print brief and params.
     */
    void print_cmd_info(Cmd *cmd, bool longer = false) const;

    /**
     * @brief read_from_file
     * @param filename
     * @return false if file could not be read, true otherwise.
     *
     * @note will remove informative output, so even cmds are read, they may
     * not have worked.
     */
    bool read_from_file(const string& filename);

    shared_ptr<Hotel> hotel_;
    string prompt_;
    bool can_start;

    // Vector that stores all cmd info.
    // Exceptionally the lines below may exceed 80 characters,
    // but otherwise the text would be less readable.   
    vector<Cmd> cmds_ =
    {
        {{"QUIT", "Q"}, "Quit",{},nullptr},
        {{"HELP", "H"},"Help",{"function"},nullptr},
        {{"READ_FROM", "RF"}, "Read", {"filename"},nullptr},
        {{"SET_DATE", "SD"},"Set date",{"day","month","year"},&Hotel::set_date},
        {{"ADVANCE_DATE", "AD"},"Advance date",{"amount"},&Hotel::advance_date},

        {{"ROOMS", "R"},"Print all rooms",{},&Hotel::print_rooms},
        {{"BOOK", "B"},"Take guest to hotel",{"guest id", "room size"},&Hotel::book},
        {{"LEAVE", "L"},"Take guest from hotel",{"guest id"},&Hotel::leave},
        {{"PRINT_GUEST_INFO", "PGI"},"Print guest's info",{"guest id"},&Hotel::print_guest_info},
        {{"PRINT_ALL_VISITS", "PAV"},"Print all visits",{},&Hotel::print_all_visits},
        {{"PRINT_CURRENT_VISITS", "PCV"},"Print current visits",{},&Hotel::print_current_visits},
        {{"PRINT_HONOR_GUESTS", "PHG"},"Print honor guests",{},&Hotel::print_honor_guests},
    };
};

#endif // CLI_HH
