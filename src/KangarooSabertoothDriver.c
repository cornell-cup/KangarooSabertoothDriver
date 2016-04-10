/**
 * File Name: 2Sabers.c
 * Description: Test controlling 2 Kangaroos using the Edison
 */

#include <stdio.h>
#include <stdlib.h>
#include "COMMAND_LIST.h"
#include "Kangaroo_Driver_Lib.h"
#include "mraa.h"
#include <unistd.h>

int main()
{
    //Variable pinA selects which device the Edison
    //is communicating with. It represents pin 13
    //pinA = 0 - Edison communicates with Kangaroos
    //pinA = 1 - Edison communicates with computer/FPGA
    mraa_gpio_context pinRead = NULL;
    mraa_gpio_context pinWrite = NULL;

    //initialize pin
    pinRead = mraa_gpio_init(13);
    pinWrite = mraa_gpio_init(12);

    if (pinRead == NULL || pinWrite == NULL) {
        fprintf(stderr, "MRAA couldn't initialize GPIO, exiting");
        return MRAA_ERROR_UNSPECIFIED;
    }

    //set the pin as output
    if (mraa_gpio_dir(pinRead, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
        fprintf(stderr, "Can't set digital pin as output, exiting");
        return MRAA_ERROR_UNSPECIFIED;
    };
    if (mraa_gpio_dir(pinWrite, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
            fprintf(stderr, "Can't set digital pin as output, exiting");
            return MRAA_ERROR_UNSPECIFIED;
        };

    //Set up the uart connection
    mraa_uart_context uart0 = uart_setup();

    //Set up Kangaroos parameters
    uint8_t address1 = 128;
    uint8_t channelName1_1 = '1';
    uint8_t channelName1_2 = '2';
    uint8_t address2 = 129;
    uint8_t channelName2_1 = '1';
    uint8_t channelName2_2 = '2';

    //Edison communicates with Kangaroos
    mraa_gpio_write(pinRead, MUX_KANGAROO);
    mraa_gpio_write(pinRead, MUX_KANGAROO);

    //Clear the Read buffer from the Kangaroos
    clearRead(uart0);

    //Start the Kangaroos channels
    start_channel(uart0, address1, channelName1_1);
    start_channel(uart0, address1, channelName1_2);
    start_channel(uart0, address2, channelName2_1);
    start_channel(uart0, address2, channelName2_2);

    //Clear the Read buffer from the Kangaroos
    clearRead(uart0);

    int32_t ui[4];

    while(1){

        //Clear the uart
        //clearRead(uart0);

        //Edison communicates with computer
        mraa_gpio_write(pinRead, MUX_COMPUTER);
        mraa_gpio_write(pinWrite, MUX_KANGAROO);

        //Read data from computer
        //While data is not available, do nothing
        while(!mraa_uart_data_available(uart0, 0)) {
            //fprintf(stdout, "Waiting for data...\n");
        }

        //When data becomes available, store it into readBuffer and print speed values
        int32_t speeds[4] = {0};
        readMotors(uart0, speeds);
        fprintf(stdout, "Write speeds %d %d %d %d:", speeds[0], speeds[1], speeds[2], speeds[3]);

        //Switch to Kangaroo Communication
        //mraa_gpio_write(pinRead, );

        //Set speeds on motors
        writeMoveSpeed(uart0, address1, channelName1_1, speeds[0]);
        writeMoveSpeed(uart0, address1, channelName1_2, speeds[1]);
        writeMoveSpeed(uart0, address2, channelName2_1, speeds[2]);
        writeMoveSpeed(uart0, address2, channelName2_2, speeds[3]);

        //Clear the Read buffer from the Kangaroos
        //clearRead(uart0);

        //Read speeds
        //readMoveSpeed(uart0, address1, channelName1_1);
        //readMoveSpeed(uart0, address1, channelName1_2);
        //readMoveSpeed(uart0, address2, channelName2_1);
        //readMoveSpeed(uart0, address2, channelName2_2);
    }

    //Destroy the uart context
    uart_destroy(uart0);

    return 0;
}
