/*
 * scan.c
 *
 *  Created on: Dec 5, 2017
 *      Author: anngould
 */

#include <scan.h>
#include <math.h>
#include <Timer.h>
#include <servo.h>
#include <adc_init.h>
#include <sonar.h>
#include <uart.h>
#include <stdbool.h>

int16_t scan_degrees = 0;
int8_t obj_count = 0;
_Bool obj_detection = false;
int16_t deg_start = 0;
int16_t deg_end = 0;
int16_t temp_deg_end = 0;
int16_t deg_width = 0;
float distance_total = 0;
float distance_avg = 0;
int16_t distance_count = 0;
float object_width = 0;
int16_t deg_smallest_obj = 0;
float smallest_obj_width = 100;
int8_t edge_count = 0;
_Bool end_case = false;

/// Resets all the global variables.
/**
 *      Resets all the global variables for a new scan. This allows for each scan to produce fresh results
 *      that are not affected by previous scans.
 *
 */
void scan_reset(){
    scan_degrees = 0;
    obj_detection = false;
    deg_start = 0;
    deg_end = 0;
    temp_deg_end = 0;
    deg_width = 0;
    distance_total = 0;
    distance_avg = 0;
    distance_count = 0;
    object_width = 0;
    deg_smallest_obj = 0;
    smallest_obj_width = 100;
    edge_count = 0;
    end_case = false;

    // Sets servo back to reset zero, waits to prevent false readings
    servo_t(0);
    timer_waitMillis(250);
}

/// Scans from 0 degrees to 180 degrees and returns over UART objects detected and their approximate location.
/**
 *      Scans from 0 degrees to 180 degrees and returns over UART objects detected and their approximate location.
 *      Sends back data in following format, space delimited:
 *              ['S'][deg_start][deg_end][distance]
 *
 */
void scan_action(){
    int degrees = 0;
      int store_degrees = 0; //stores degrees before an object is being detected :)
      int objects[15];
      int real = 0;
      int f_location[15];
      int b_location[15];
      int num_value = 0;
      int on_object = 0;

          while(degrees < 170){
               int avg = ir_stuff();

               int ping_distance = ping_read();
                  if( avg < 50){
                      real++;
                      if(on_object == 0 && real >= 3 ) {
                          store_degrees = degrees;
                          f_location[num_value] = degrees;
                          on_object = 1;
                      }
                  }
                  else if(on_object == 1){
                      objects[num_value] = degrees - store_degrees;
                      b_location[num_value] = degrees;
                      lcd_printf("%d", ping_read());

                      // send distance back
                      char my_str[30];
                      sprintf(my_str, "distance: %d, start: %d, end: %d\n", ping_distance, degrees, store_degrees);
                      send_string(my_str);

                      num_value++;
                      on_object = 0;
                      real = 0;
                  }
                  else{
                      real = 0;
                  }
              servo_t(degrees);
              timer_waitMillis(10);
              degrees += 1;
          }
          int i;
          int max = objects[0];
          int max_index = 0;
          int min_index = 0;
          int min = objects[0];
          for(i=0; i < num_value; i++){
              if(objects[i] > max){
                  max = objects[i];
                  max_index = i;
              }

              if(objects[i] < min){
                  min = objects[i];
                  min_index = i;
              }
          }
          servo_t(b_location[min_index] - (objects[min_index]));
          lcd_printf("%d", min_index);

}

