/*                      *\
    Copyrights 2025
            Riombotique
\*                      */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*   Code du RPI Pico principale gérant les differentes entrées-sorties.             *
*   Ce Pico est un maitre pilotant le gyroscope, l'internet et le motion controller.*
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "include/robot.h"

robot_t robot;

int main(void)
{
    if(robot_init()) return -1;

    while(robot.is_running)
    {
        robot_handle_inputs_outputs();
    }

    robot_deinit();

    return 0;
}
