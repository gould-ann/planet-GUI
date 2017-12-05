/*
 * movement.c
 *
 *  Created on: Sep 7, 2017
 *      Author: demoss
 */

#include "movement.h"
#include "lcd.h"
#include "Timer.h"
#include "uart.h"


/*
 * Simple movement function (+N) is forward motion (-N) is backward motion
 * Note that the CyBot will turn (+ or -)90 degrees if a Front sensor is detected
 */
void move(oi_t *sensor, int centimeters, int velocity_mag)
{
    oi_setWheels((centimeters > 0) ? velocity_mag : -1*velocity_mag, (centimeters > 0) ? velocity_mag : -1*velocity_mag);
    int sum = 0;
        while (abs(sum)<abs(centimeters*10))
        {
            oi_update(sensor);
            sum += sensor->distance;
        }
        oi_setWheels(0, 0); // stop
}

/*
 * Simple turn function (+N) is counter-clockwise (-N) is clockwise
 */
void turn(oi_t *sensor, int degrees)
{
    lcd_init();
   // char msg[81] = "";
    oi_setWheels((degrees > 0) ? 100 : -100, (degrees > 0) ? -100 : 100);
    int sum = 0;
    while(abs(sum)<abs(degrees))
    {
        oi_update(sensor);
        sum += sensor->angle;
        //sprintf(msg, "Angle turned:\n%d", sum);
       // lcd_printf(msg);
    }
    oi_setWheels(0, 0); // stop
}

/*
 * Moves the bot forward or backward N centimeter
 * e.g. "collision_avoidance(sensor, N);" --> if N = 15 moves forward 15cm.  if N = -15 moves backward 15cm
 * Integrated CRATER avoidance and WHITE LINE avoidance
 * When hits a CRATER or WHITE LINE moves backward 5cm and turns + or - 90 degrees
 *
 */

void auto_reroute(oi_t *sensor, int centimeters){
     //include lcd_init() for print testing
     lcd_init();
     oi_setWheels((centimeters > 0) ? 300 : -298.5, (centimeters > 0) ? 300 : -298.5);
     char msg[81] = "";
     int sum = 0;

    //IMPORTANT: (TODO)
    //This variable is the distance left in the forward (x) direction after the CyBot has hit a BUMP, CRATOR or WHITE LINE
    //Note we will need this value for mapping

    int x = 0;
    int milimeters = centimeters*10;
        while (abs(sum)<abs(milimeters))
        {
            oi_update(sensor);
            sum += sensor->distance;

            if(sensor->bumpLeft == 1)
            {
                move(sensor,-5,250);
                turn(sensor,-90);
                x = (sum/10);
                sum = abs(milimeters);
                sprintf(msg, "centimeters traveled before Front Left BUMP\n%d", x);
                send_string(msg);
                lcd_printf(msg);
            }

            else if(sensor->bumpRight == 1)
            {
                move(sensor,-5,250);
                turn(sensor,90);
                x = (sum/10);
                sum = abs(milimeters);
                sprintf(msg, "centimeters traveled before Front Right BUMP\n%d", x);
                send_string(msg);
                lcd_printf(msg);
            }


            //The values in the following group indicate CRATERs
            //Note that the CyBot will turn (+ or -)90 degrees if a Front sensor is detected
            //and will turn (+ or -)45 degrees if a regular (side) sensor is detected

            //These values indicate CRATERs
            else if(sensor-> cliffFrontRightSignal < 800)
            {
                move(sensor, -5, 250);
                turn(sensor, 90);
                x = (sum/10);
                sprintf(msg, "centimeters traveled before CRATER on Front Right\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if(sensor-> cliffRightSignal < 800)
            {
                move(sensor, -5, 250);
                turn(sensor, 45);
                x = (sum/10);
                sprintf(msg, "centimeters traveled before CRATER on Right\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if(sensor-> cliffFrontLeftSignal < 800)
            {
                move(sensor, -5, 250);
                turn(sensor, -90);
                x = (sum/10);
                sprintf(msg, "centimeters traveled before CRATER on Front Left\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if(sensor-> cliffLeftSignal < 800)
            {
                move(sensor, -5, 250);
                turn(sensor, -45);
                x = (sum/10);
                sprintf(msg, "centimeters traveled before CRATER on Left\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            //The values in the following group indicate WHITE LINEs

            else if((sensor->cliffFrontRight) || ((sensor->cliffFrontRightSignal) > 2750))
            {
                move(sensor, -5, 250);
                turn(sensor, 90);
                x = (sum/10);
                char msg[81] = "";
                sprintf(msg, "centimeters traveled before WHITE LINE on Front Right\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }


            else if((sensor->cliffRight) || ((sensor->cliffRightSignal) > 2750))
            {
                move(sensor, -5, 250);
                turn(sensor, 45);
                x = (sum/10);
                char msg[81] = "";
                sprintf(msg, "centimeters traveled before WHITE LINE on Right\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if((sensor->cliffFrontLeft) || ((sensor->cliffFrontLeftSignal) > 2750)){
                move(sensor, -5, 250);
                turn(sensor, -90);
                x = (sum/10);
                char msg[81] = "";
                sprintf(msg, "centimeters traveled before WHITE LINE on Front Left\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if((sensor->cliffLeft) || ((sensor->cliffLeftSignal) >2750))
            {
                move(sensor, -5, 250);
                turn(sensor, -45);
                x = (sum/10);
                char msg[81] = "";
                sprintf(msg, "centimeters traveled before WHITE LINE on Left\n%d", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

        }
        oi_setWheels(0, 0); // stop
}
