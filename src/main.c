/**
  ******************************************************************************
  * @file    main.c
  * @author  Bardina VA
  * @version CMSIS
  ******************************************************************************
*/

#include "stm32f10x.h"

int main(void)
{
	//Включаем тактирование порта C
	RCC ->APB2ENR |= RCC_APB2ENR_IOPCEN;

	//светодиод - порт C, пин13 (high - CRH)
	//сброс состояния
	//вывод светодиода как выход pull/push MODE=10: Max. output speed 2 MHz ; CRH=00:  General purpose output push-pull
	GPIOC -> CRH &= ~(GPIO_CRH_CNF13_1 | GPIO_CRH_CNF13_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);
	GPIOC -> CRH |= GPIO_CRH_MODE13_1;

	for(;;);
}
