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
	//GPIOC -> APB2; тактирование - rcc -> stm32f10x_rcc.h => void RCC_APB2PeriphClockCmd
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// включение вывода 13 на выход.
	//mode=10 (Output mode, max speed 2 MHz); cnf = 00 (General purpose output push-pull)
	//Push-Pull Mode: A “0” in the Output register activates the N-MOS while a “1” in the Output register activates the P-MOS
	GPIO_InitTypeDef PinC13;
	PinC13.GPIO_Pin = GPIO_Pin_13;
	PinC13.GPIO_Speed =  GPIO_Speed_2MHz;
	PinC13.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &PinC13);
	//обнулить загорание
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);

	// включаем тактирование порта А
	//0x40021400 rcc (datasheet)
	//address rcc_apb2enr = 0x18
	//2 - iopaen
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//включение пина 2 на вход + сброс состояние портов
	//mode - 00: Input mode (reset state); cnf - 10: Input with pull-up / pull-down
	//pin 2 - low (11, 10 (cnf)), 9, 8 (mode))
	//0x40010C00 gpioc (datasheet)
	//address low - 0x00, reset value - 0x44444444
	GPIO_InitTypeDef PinA2;
	PinA2.GPIO_Pin = GPIO_Pin_2;
	PinA2.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA2);

	//прописываем подтяжку к земле
	//0x40010C00 gpioc (datasheet)
	//address output data register - 0x0C
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);

	for(;;)
	{
		//если питание
		//address input data register - 0x08
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
		//зажечь
		{
					GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}
		//иначе
		//потухнуть
		else
		{
				GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}
	};
}
