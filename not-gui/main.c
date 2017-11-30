#include <stdint.h>
#include "lcd.h"
#include "Timer.h"
#include "button.h"
#include "WiFi.h"
#include "uart.h"

void UART_init(void);
//void UART_Transmit(char sdata);
char UART_Receive(void);
char uart_receive(void);

//30923764.5 0x1D7DBF4
//prelab 2: 8 & 44



//Blocking call to receive over uart1
//returns char with data
//int last = 0;
int main(void)
{

    char buffer[21];
    int i = 0;
    int j = 0;
    UART_init();
    button_init();
    lcd_init();
    char pass[] = "annIsCool123";
    char temp;
   // WiFi_start(pass);
    while(1){
        temp = uart_receive();
        if(temp != '\r'){
            lcd_printf("the char is: %c\n the index is %d", temp, i);
            buffer[i] = temp;

            //PSUEDO
            //SET wheels
            //if(temp == 'r')
            //rotate
            //if(temp == 'u')
//            i++;
//            if(i==20){
//                lcd_printf("%s\n", buffer);
//                uart_sendChar('\n');
//                uart_sendChar('\r');
//                i = 0;
//                for(j = 0; j < 20; j++){
//                    uart_sendChar(buffer[j]);
//                    buffer[j] = '\0';
//                }
//            }

        } else {
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
