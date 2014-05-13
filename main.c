#include "stm32f4xx_conf.h"
#include "misc.h"
#include "config.h"

int main(void)
{
	//Setup Configuration Files
	configInit();

	while (1)
	{
	  	char getChar = Config.serial.getChar();
		if(getChar != 0x00)
		{
			Config.serial.send(&getChar);
		}   
	} 
}

