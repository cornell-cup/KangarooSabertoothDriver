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
#include <time.h>       //for time tracking
#include "uart2.h"

void delay(int milliseconds);

int main()
{

    /*mraa_gpio_context pinRead = NULL;
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

    while(0){

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
        //mraa_gpio_write(pinRead, MUX_KANGAROO);

        //Set speeds on motors
        writeMoveSpeed(uart0, address1, channelName1_1, speeds[0]);
        writeMoveSpeed(uart0, address1, channelName1_2, speeds[1]);
        writeMoveSpeed(uart0, address2, channelName2_1, speeds[2]);
        writeMoveSpeed(uart0, address2, channelName2_2, speeds[3]);

        //Clear the Read buffer
        //clearRead(uart0);

        //Read speeds
        //readMoveSpeed(uart0, address1, channelName1_1);
        //readMoveSpeed(uart0, address1, channelName1_2);
        //readMoveSpeed(uart0, address2, channelName2_1);
        //readMoveSpeed(uart0, address2, channelName2_2);
    }

    //Code to measure time to set and read speeds on motors

    int32_t speeds[4] = {4000,-4000,-4000,4000};
    clock_t timeWrite;
    timeWrite = clock();
    writeMoveSpeed(uart0, address1, channelName1_1, speeds[0]);
    writeMoveSpeed(uart0, address1, channelName1_2, speeds[1]);
    writeMoveSpeed(uart0, address2, channelName2_1, speeds[2]);
    writeMoveSpeed(uart0, address2, channelName2_2, speeds[3]);
    timeWrite = clock() - timeWrite;
    double time_taken_write = ((double)timeWrite)/CLOCKS_PER_SEC; // in seconds

    printf("Took %f seconds to set 4 speeds \n", time_taken_write);
    // Took 0.000152 seconds to set 4 speeds
    // Took 0.000099 seconds to set 4 speeds
    // Took 0.000095 seconds to set 4 speeds
    // Took 0.000157 seconds to set 4 speeds

    delay(250);

    clock_t timeRead;
    timeRead = clock();
    readMoveSpeed(uart0, address1, channelName1_1);
    readMoveSpeed(uart0, address1, channelName1_2);
    readMoveSpeed(uart0, address2, channelName2_1);
    readMoveSpeed(uart0, address2, channelName2_2);
    timeRead = clock() - timeRead;
    double time_taken_read = ((double)timeRead)/CLOCKS_PER_SEC; // in seconds

    printf("Took %f seconds to read 4 speeds \n", time_taken_read);
    // Took 0.001289 seconds to read 4 speeds
    // Took 0.001397 seconds to read 4 speeds
    // Took 0.001263 seconds to read 4 speeds
    // Took 0.001342 seconds to read 4 speeds

    //Destroy the uart context
    uart_destroy(uart0);*/

    // Code to test uart2

    mraa_uart_context uart1;

    if (detach_console()) {
        fprintf(stdout, "Failed to detach system console.\n");
        fflush(stdout);
        return EXIT_FAILURE;
    }

    uart1 = mraa_uart_init_raw("/dev/ttyMFD2");
    if (uart1 == NULL) {
        fprintf(stdout, "UART2 failed to setup\n");
        return EXIT_FAILURE;
    }
    else{
        printf("UART2 initialized\n");
    }
    mraa_uart_set_mode(uart1, 8,MRAA_UART_PARITY_NONE , 1);
    mraa_uart_set_baudrate(uart1, 9600);

    char buffer[] = "\nHello Mraa!\n";
    mraa_uart_write(uart1, buffer, sizeof(buffer));

    //clearRead(uart1);


   // while(mraa_uart_data_available(uart1, 0) == 0){
        //do nothing
        printf("waiting for data\n");
    //}

    char stop = 0;
    //while(!stop){
        char read[4];
        if(mraa_uart_data_available(uart1, 0) == 0){
            printf("No Data\n");
        }
        else{
            printf("DATA READ");
            stop = 1;
        }
  //  }

    reattach_console();
    fprintf(stdout, "Console reattached.\n");


    return 0;
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}
