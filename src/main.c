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
	//Включаем тактирование порта A и C
	RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

	//кнопка 1 - порт A, пин6 (low - CRL)
	//сброс состояния
	GPIOA -> CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0);
	//вывод кнопки как выход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	GPIOA -> CRL |= GPIO_CRL_CNF6_1;
	GPIOA->ODR &= ~GPIO_ODR_ODR6; //к земле

	//кнопка 2 - порт A, пин8 (high - CRH)
	//сброс состояния
	GPIOA -> CRH &= ~(GPIO_CRH_CNF8_1 | GPIO_CRH_CNF8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0);
	//вывод кнопки как выход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	GPIOA -> CRH |= GPIO_CRH_CNF8_1;
	GPIOA->ODR &= ~GPIO_ODR_ODR8; //к земле

	//светодиод - порт C, пин13 (high - CRH)
	//сброс состояния
	//вывод светодиода как выход pull/push MODE=10: Max. output speed 2 MHz ; CRH=00:  General purpose output push-pull
	GPIOC -> CRH &= ~(GPIO_CRH_CNF13_1 | GPIO_CRH_CNF13_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);
	GPIOC -> CRH |= GPIO_CRH_MODE13_1;

	for(;;);
}
