/**
  ******************************************************************************
  * @file    main.c
  * @author  Bardina VA
  * @version CMSIS
  ******************************************************************************
*/

#include "stm32f10x.h"

uint16_t delay = 500;
uint16_t work = 500;

int main(void)
{
	//Включаем тактирование порта A, B и C
	RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;

	//кнопка 1 - порт A, пин6 (low - CRL)
	//сброс состояния
	GPIOA -> CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	GPIOA -> CRL |= GPIO_CRL_CNF6_1;
	GPIOA->ODR &= ~GPIO_ODR_ODR6; //к земле

	//кнопка 2 - порт A, пин8 (high - CRH)
	//сброс состояния
	GPIOA -> CRH &= ~(GPIO_CRH_CNF8_1 | GPIO_CRH_CNF8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	GPIOA -> CRH |= GPIO_CRH_CNF8_1;
	GPIOA->ODR &= ~GPIO_ODR_ODR8; //к земле

	//кнопка 3 - порт A, пин3 (low - CRL)
	//сброс состояния
	GPIOA -> CRL &= ~(GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	GPIOA -> CRL |= GPIO_CRL_CNF3_1;
	GPIOA -> ODR |= GPIO_ODR_ODR3; //к питанию

	//кнопка 4 - порт B, пин12 (high - CRH)
	//сброс состояния
	GPIOB -> CRH &= ~(GPIO_CRH_CNF12_1 | GPIO_CRH_CNF12_0 | GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	GPIOB -> CRH |= GPIO_CRH_CNF12_1;
	GPIOB -> ODR |= GPIO_ODR_ODR12; //к питанию

	//светодиод - порт C, пин13 (high - CRH)
	//сброс состояния
	//вывод светодиода как выход pull/push MODE=10: Max. output speed 2 MHz ; CRH=00:  General purpose output push-pull
	GPIOC -> CRH &= ~(GPIO_CRH_CNF13_1 | GPIO_CRH_CNF13_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);
	GPIOC -> CRH |= GPIO_CRH_MODE13_1;
	//обнулить загорание
	GPIOC->ODR |= GPIO_ODR_ODR13;

	//включаем тактирование таймера
	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;

	//запускаем таймер на тактовой частоте в 1000 Hz
	//Fmax = 36 000 000 Hz, нужно 1000 => 36 0000 000 / 1 000 = 36 000
	TIM3 -> PSC = 36000 - 1; //задаем предделитель TIMx_PSC
	//начальное значение периода - 500 тактов (delay)
	TIM3 -> ARR = delay-1;
	//разрешаем прерывания таймера 3 по переполнению (DMA/interrupt enable register ;  Update interrupt enable)
	TIM3 -> DIER |= TIM_DIER_UIE;

	// Включение прерывания таймера 3
	NVIC_EnableIRQ(TIM3_IRQn);

	//включем таймер
	TIM3->CR1 |= TIM_CR1_CEN;

	for(;;)
	{
		//условие для кнопки 2
		if ( GPIOA->IDR & GPIO_IDR_IDR8 )
		{
			work = 1500-1;
		}
		//условие для кнопки 3
		else if(!(GPIOA->IDR & GPIO_IDR_IDR3))
		{
			 work = 2500-1;
		}
		//условие для кнопки 4
		else if(!(GPIOB->IDR & GPIO_IDR_IDR12))
		{
			work = 3500-1;
		}
		else
		{
			work = 500-1;
		}
	};
}

void TIM3_IRQHandler(void)
{
	 TIM3->SR &= ~TIM_SR_UIF; //Clean UIF Flag

		if ( GPIOC->ODR & GPIO_ODR_ODR13 )
		{
			TIM3 -> ARR = work-1;
			GPIOC->ODR &= ~ GPIO_ODR_ODR13;
		}
		else
		{
			TIM3 -> ARR = delay-1;
			GPIOC->ODR |= GPIO_ODR_ODR13;
		}

}
