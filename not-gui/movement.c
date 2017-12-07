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
#include "string.h"


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
     oi_setWheels((centimeters > 0) ? 100 : -100, (centimeters > 0) ? 100 : -100);
     char msg[81] = "none";
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

            x = (sum/10);

            if(sensor->bumpLeft == 1)
            {
                move(sensor,x * -1,250);
              //  turn(sensor,-90);
                sum = abs(milimeters);
                sprintf(msg, "{\"class\":1,\"type\": \"bump-left\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
            }

            else if(sensor->bumpRight == 1)
            {
                move(sensor,x * -1,250);
              //  turn(sensor,90);
                sum = abs(milimeters);
                sprintf(msg, "{\"class\":1,\"type\": \"bump-right\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
            }


            //The values in the following group indicate CRATERs
            //Note that the CyBot will turn (+ or -)90 degrees if a Front sensor is detected
            //and will turn (+ or -)45 degrees if a regular (side) sensor is detected

            //These values indicate CRATERs
            else if(sensor-> cliffFrontRightSignal < 800)
            {
                move(sensor, x * -1, 250);
               // turn(sensor, 90);
                sprintf(msg, "{\"class\":1,\"type\": \"cliff-right\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if(sensor-> cliffRightSignal < 800)
            {
                move(sensor, x * -1, 250);
               // turn(sensor, 45);
                sprintf(msg, "{\"class\":1,\"type\": \"cliff-right\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if(sensor-> cliffFrontLeftSignal < 800)
            {
                move(sensor, x * -1, 250);
               // turn(sensor, -90);
                sprintf(msg, "{\"class\":1,\"type\": \"cliff-left\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if(sensor-> cliffLeftSignal < 800)
            {
                move(sensor, x * -1, 250);
               // turn(sensor, -45);
                sprintf(msg, "{\"class\":1,\"type\": \"cliff-left\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            //The values in the following group indicate WHITE LINEs

            else if((sensor->cliffFrontRight) || ((sensor->cliffFrontRightSignal) > 2750))
            {
                move(sensor, x * -1, 250);
               // turn(sensor, 90);
                char msg[81] = "";
                sprintf(msg, "{\"class\":1,\"type\": \"white-line\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }


            else if((sensor->cliffRight) || ((sensor->cliffRightSignal) > 2750))
            {
                move(sensor, x * -1, 250);
              //  turn(sensor, 45);
                char msg[81] = "";
                sprintf(msg, "{\"class\":1,\"type\": \"white-line\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if((sensor->cliffFrontLeft) || ((sensor->cliffFrontLeftSignal) > 2750)){
                move(sensor, x * -1, 250);
               // turn(sensor, -90);
                char msg[81] = "";
                sprintf(msg, "{\"class\":1,\"type\": \"white-line\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }

            else if((sensor->cliffLeft) || ((sensor->cliffLeftSignal) >2750))
            {
                move(sensor, x * -1, 250);
               // turn(sensor, -45);
                char msg[81] = "";
                sprintf(msg, "{\"class\":1,\"type\": \"white-line\", \"distance\": %d}\n", x);
                send_string(msg);
                lcd_printf(msg);
                sum = abs(milimeters);
            }


        }

        if(strcmp(msg, "none") == 0){
            sprintf(msg, "{\"class\":0, \"type\": \"move_forward\", \"distance\": %d}\n", x);
            send_string(msg);
        }
        oi_setWheels(0, 0); // stop
        }
