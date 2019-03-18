/**
  ******************************************************************************
  * @file    Bardina
  * @version V1.0
  ******************************************************************************
*/


#include "stm32f10x.h"
			

int main(void)
{
	// Включаем тактирование порта C


	// включение вывода 13 на выход.
	//mode=10 (Output mode, max speed 2 MHz); cnf = 00 (General purpose output push-pull)
	//Push-Pull Mode: A “0” in the Output register activates the N-MOS while a “1” in the Output register activates the P-MOS

	//обнулить загорание

	// включаем тактирование порта А
	//0x40021400 rcc (datasheet)
	//address rcc_apb2enr = 0x18
	//2 - iopaen

	//включение пина 2 на вход + сброс состояние портов
	//mode - 00: Input mode (reset state); cnf - 10: Input with pull-up / pull-down
	//pin 2 - low (11, 10 (cnf)), 9, 8 (mode))
	//0x40010C00 gpioc (datasheet)
	//address low - 0x00, reset value - 0x44444444

	//прописываем подтяжку к земле
	//0x40010C00 gpioc (datasheet)
	//address output data register - 0x0C

	for(;;)
	{
		//если питание
		//address input data register - 0x08
		//зажечь
		//иначе
		//потухнуть
	};
}
