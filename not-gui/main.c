#include <stdint.h>
#include "lcd.h"
#include "Timer.h"
#include "button.h"
#include "WiFi.h"
#include "uart.h"
#include "movement.h"
#include "open_interface.h"

void UART_init(void);
//void UART_Transmit(char sdata);
char UART_Receive(void);
char uart_receive(void);
int sweep(void);
//30923764.5 0x1D7DBF4
//prelab 2: 8 & 44



//Blocking call to receive over uart1
//returns char with data
//int last = 0;
int main(void)
{

    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    oi_shutoff_init();
    GPIOF_Handler();
    char buffer[21];
    int i = 0;
    int j = 0;
    lcd_init();
    UART_init();
    button_init();

    char pass[] = "annIsCool123";
    char temp;
    //WiFi_start(pass);
    while(1){
        temp = uart_receive();
        if(temp != '\r'){
            lcd_printf("the char is: %c\n the index is %d", temp, i);
            buffer[i] = temp;
            if(temp == 'u'){
                auto_reroute(sensor, 25);
            }
            if(temp =='r'){
              turn(sensor, -90);
            }
            if(temp == 'l'){
                lcd_printf("yay!");
               turn(sensor, 90);

            }
            if(temp == 'd'){
                move(sensor, -10, 100);
            }
            if(temp=='s'){

            }
        }

        else {
            buffer[i] = '\0';
            i = 0;
            lcd_printf("%s\n", buffer);
            uart_sendChar('\n');
            uart_sendChar('\r');
            for(j = 0; j < 20; j++){
               uart_sendChar(buffer[j]);
             }

        }


    }
    return 0;
}

int sweep(void){

    int degrees = 0;
    while(degrees < 170){
         int avg = ir_stuff();
         int ping_distance = ping_dist()/10;
        lcd_printf("%d ir dist\n %d ping dist\n %d degrees\n %d largest objs\n", avg, ping_distance, degrees, num_value);

        char to_char_array[7];
//        send_string("            Degrees:");
//        snprintf(to_char_array, 7, "%d", degrees);
//        send_string(to_char_array);

//        send_string("            Ir Distance (cm):");
//        snprintf(to_char_array, 7, "%d", avg);
//        send_string(to_char_array);

//        send_string("            Sonar Distance (cm):");
//        snprintf(to_char_array,7, "%d", ping_distance);
//        send_string(to_char_array);
        uart_sendChar('\n');
        uart_sendChar('\r');

      // values[num_value % 3] = ir_stuff();
//      num_value++;

//      if(num_value > 2){
//
//          //int avg = (values[0]+values[1]+values[2])/3;
//
            if( avg < 80){
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
        if(degrees >= 170){
            break;
        }
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
