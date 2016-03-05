#include <stdio.h>
#include <stdlib.h>


#include <stdio.h>
#include <stdlib.h>
#include "COMMAND_LIST.h"
#include "Kangaroo_Driver_Lib.h"
#include "mraa.h"
#include <unistd.h>

int main(){
	//Set up the uart connection
	mraa_uart_context uart0 = uart_setup();

	//Set up kangaroo params
	uint8_t address = 128;
	uint8_t channelName = '1';


	start_channel(uart0, address, channelName);


	int32_t vels[11] = {-5000, -4000, -3000, -2000, -1000, 0 ,1000 ,2000, 3000, 4000, 5000};

	for(int i = 0; i <= 10; i++){
		writeMoveSpeed(uart0, address, channelName, vels[i]);
		sleep(5);
	}

	fprintf(stdout,"yo");


	//Destroy the uart context
	uart_destroy(uart0);


	return 0;
}
