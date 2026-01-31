/*                      *\
    Copyrights 2025
            Riombotique
\*                      */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*   Code du RPI Pico gérant les different Actionneurs.      *
*   Ce Pico est un esclave piloté par le Pico Principal.    *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>
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
