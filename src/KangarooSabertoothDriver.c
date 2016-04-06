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
	//Variable "A" selects which device the Edison
	//is communicating with. It represents pin 13
	//A = 0 - Edison communicates with Kangaroos
	//A = 1 - Edison communicates with computer/FPGA
	mraa_gpio_context A = NULL;
	//initialize pin
	A = mraa_gpio_init(13);
	if (A == NULL) {
		fprintf(stderr, "MRAA couldn't initialize GPIO, exiting");
		return MRAA_ERROR_UNSPECIFIED;
	}
	//set the pin as output
	if (mraa_gpio_dir(A, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
		fprintf(stderr, "Can't set digital pin as output, exiting");
		return MRAA_ERROR_UNSPECIFIED;
	};

	//Edison communicates with Kangaroo
	mraa_gpio_write(A, 0);

	//Set up the uart connection
	mraa_uart_context uart0 = uart_setup();

	//Set up Kangaroos parameters
	uint8_t address1 = 128;
	uint8_t channelName1_1 = '1';
	uint8_t channelName1_2 = '2';
	uint8_t address2 = 129;
	uint8_t channelName2_1 = '1';
	uint8_t channelName2_2 = '2';

	//readMoveSpeed(uart0, address, channelName);
	//Clear the Read buffer from the Kangaroos
	clearRead(uart0);

	//Start the Kangaroos channels
	start_channel(uart0, address1, channelName1_1);
	start_channel(uart0, address1, channelName1_2);
	start_channel(uart0, address2, channelName2_1);
	start_channel(uart0, address2, channelName2_2);

	sleep(5);

	//readMoveSpeed(uart0, address, channelName);
	//Clear the Read buffer from the Kangaroos
	clearRead(uart0);

	int32_t vels[4] = {-4000, -2000, 2000, 4000};

	/*writeMoveSpeed(uart0, address1, channelName1_1, vels[0]);
	sleep(3);
	fprintf(stdout, "Read Speed on Kangaroo 128, channel 1: \n");
	readMoveSpeed(uart0, address1, channelName1_1);
	sleep(5);
	writeMoveSpeed(uart0, address1, channelName1_2, vels[2]);
	sleep(3);
	fprintf(stdout, "Read Speed on Kangaroo 128, channel 2: \n");
	readMoveSpeed(uart0, address1, channelName1_2);
	sleep(5);
	writeMoveSpeed(uart0, address2, channelName2_1, vels[3]);
	sleep(3);
	fprintf(stdout, "Read Speed on Kangaroo 129, channel 1: \n");
	readMoveSpeed(uart0, address2, channelName2_1);
	sleep(5);
	writeMoveSpeed(uart0, address2, channelName2_2, vels[1]);
	sleep(3);
	fprintf(stdout, "Read Speed on Kangaroo 129, channel 2: \n");
	readMoveSpeed(uart0, address2, channelName2_2);
	sleep(5);*/

	int32_t ui[4];
	int32_t stop = 0;

	char buffer[] = "Hello!";

	while(stop != 1){

		int choice = -1;
		do{
			fprintf(stdout, "Enter: 0 - set new speeds, 1 - send and receive data from computer:");
			scanf("%d", &choice);
		} while (choice != 0 && choice != 1);

		//Clear the Read buffer from the Kangaroos
		clearRead(uart0);

		mraa_gpio_write(A, choice);

		switch(choice) {
			case 1:
				//Send data to computer
				mraa_uart_write(uart0, buffer, sizeof(buffer));

				//Clear the Read buffer
				clearRead(uart0);

				//Read data from computer
				//While data is not available, do nothing
				while(!mraa_uart_data_available(uart0, 0)) {
					fprintf(stdout, "Waiting for data...\n");
					sleep(1);
				}
				int8_t readBuffer[4];
				mraa_uart_read(uart0, readBuffer, sizeof(readBuffer));
				fprintf(stdout, "Data received: %d %d %d %d\n", readBuffer[0], readBuffer[1], readBuffer[2], readBuffer[3]);

				//Switch to Kangaroo Communication
				mraa_gpio_write(A, 0);

				writeMoveSpeed(uart0, address1, channelName1_1, readBuffer[0]);
				writeMoveSpeed(uart0, address1, channelName1_2, readBuffer[1]);
				writeMoveSpeed(uart0, address2, channelName2_1, readBuffer[2]);
				writeMoveSpeed(uart0, address2, channelName2_2, readBuffer[3]);
				sleep(3);
				//Clear the Read buffer from the Kangaroos
				clearRead(uart0);
				readMoveSpeed(uart0, address1, channelName1_1);
				readMoveSpeed(uart0, address1, channelName1_2);
				readMoveSpeed(uart0, address2, channelName2_1);
				readMoveSpeed(uart0, address2, channelName2_2);

				fprintf(stdout, "Enter 1 to stop or 0 to continue:");
				scanf("%d", &stop);
				break;
			case 0:
				/*start_channel(uart0, address1, channelName1_1);
				start_channel(uart0, address1, channelName1_2);
				start_channel(uart0, address2, channelName2_1);
				start_channel(uart0, address2, channelName2_2);*/
				fprintf(stdout, "Enter Speeds for motors (and stop):");
				scanf("%d %d %d %d %d", &ui[0], &ui[1], &ui[2], &ui[3], &stop);
				writeMoveSpeed(uart0, address1, channelName1_1, ui[0]);
				writeMoveSpeed(uart0, address1, channelName1_2, ui[1]);
				writeMoveSpeed(uart0, address2, channelName2_1, ui[2]);
				writeMoveSpeed(uart0, address2, channelName2_2, ui[3]);
				sleep(3);
				readMoveSpeed(uart0, address1, channelName1_1);
				readMoveSpeed(uart0, address1, channelName1_2);
				readMoveSpeed(uart0, address2, channelName2_1);
				readMoveSpeed(uart0, address2, channelName2_2);
				break;
		}
	}

	//Destroy the uart context
	uart_destroy(uart0);

	return 0;
}
