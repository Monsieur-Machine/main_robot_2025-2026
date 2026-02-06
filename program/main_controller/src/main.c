//
//  Copyrights 2025
//          Poivron Robotique
//

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*   Code du RPI Pico principale gérant les differentes entrées-sorties.             *
*   Ce Pico est un maitre pilotant le gyroscope, l'internet et le motion controller.*
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "headers/robot.h"

robot_t robot;

int main(void)
{
    robot_init();

    while(robot.is_running)
    {
        robot_handle_inputs_outputs();
    }

    robot_deinit();

    return 0;
}
