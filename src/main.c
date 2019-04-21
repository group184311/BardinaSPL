/*
  ******************************************************************************
  * @file    main.c
  * @author  Bardina VA
  * @version CMSIS & STD
  ******************************************************************************
*/

#include "stm32f10x.h"

uint16_t delay = 500;
uint16_t work = 500;
uint16_t last_state, state, last_delay; //значение в прошлый и настоящий момент времени

int main(void)
{

	//Включаем тактирование порта A, B и C
	//RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//кнопка 1 - порт A, пин6 (low - CRL)
	//сброс состояния
	//GPIOA -> CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	//GPIOA -> CRL |= GPIO_CRL_CNF6_1;
	//GPIOA->ODR &= ~GPIO_ODR_ODR6; //к земле
	GPIO_InitTypeDef PinA6;
	PinA6.GPIO_Pin = GPIO_Pin_6;
	PinA6.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA6);

	//кнопка 2 - порт A, пин8 (high - CRH)
	//сброс состояния
	//GPIOA -> CRH &= ~(GPIO_CRH_CNF8_1 | GPIO_CRH_CNF8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	//GPIOA -> CRH |= GPIO_CRH_CNF8_1;
	//GPIOA->ODR &= ~GPIO_ODR_ODR8; //к земле
	GPIO_InitTypeDef PinA8;
	PinA8.GPIO_Pin = GPIO_Pin_8;
	PinA8.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA8);

	//кнопка 3 - порт A, пин3 (low - CRL)
	//сброс состояния
	//GPIOA -> CRL &= ~(GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	//GPIOA -> CRL |= GPIO_CRL_CNF3_1;
	//GPIOA -> ODR |= GPIO_ODR_ODR3; //к питанию
	GPIO_InitTypeDef PinA3;
	PinA3.GPIO_Pin = GPIO_Pin_3;
	PinA3.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &PinA3);

	//кнопка 4 - порт B, пин12 (high - CRH)
	//сброс состояния
	//GPIOB -> CRH &= ~(GPIO_CRH_CNF12_1 | GPIO_CRH_CNF12_0 | GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0);
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	//GPIOB -> CRH |= GPIO_CRH_CNF12_1;
	//GPIOB -> ODR |= GPIO_ODR_ODR12; //к питанию
	GPIO_InitTypeDef PinB12;
	PinB12.GPIO_Pin = GPIO_Pin_12;
	PinB12.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &PinB12);

	//светодиод - порт C, пин13 (high - CRH)
	//сброс состояния
	//вывод светодиода как выход pull/push MODE=10: Max. output speed 2 MHz ; CRH=00:  General purpose output push-pull
	//GPIOC -> CRH &= ~(GPIO_CRH_CNF13_1 | GPIO_CRH_CNF13_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);
	//GPIOC -> CRH |= GPIO_CRH_MODE13_1;
	GPIO_InitTypeDef PinC13;
	PinC13.GPIO_Pin = GPIO_Pin_13;
	PinC13.GPIO_Speed =  GPIO_Speed_2MHz;
	PinC13.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &PinC13);
	//обнулить загорание
	//GPIOC->ODR |= GPIO_ODR_ODR13;
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);

	//включаем тактирование таймера
	//RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//запускаем таймер на тактовой частоте в 1000 Hz
	//Fmax = 36 000 000 Hz, нужно 1000 => 36 0000 000 / 1 000 = 36 000
	//TIM3 -> PSC = 36000 - 1; //задаем предделитель TIMx_PSC
	//начальное значение периода - 500 тактов (delay)
	//TIM3 -> ARR = 2*delay-1;
	TIM_TimeBaseInitTypeDef TIM3_CLOCK;
	TIM3_CLOCK.TIM_Prescaler = 36000 - 1;
	TIM3_CLOCK.TIM_Period = 2*delay-1;
	TIM3_CLOCK.TIM_CounterMode = TIM_CounterMode_Up;
	TIM3_CLOCK.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM3_CLOCK);

	//разрешаем прерывания таймера 3 по переполнению (DMA/interrupt enable register ;  Update interrupt enable)
	//TIM3 -> DIER |= TIM_DIER_UIE;
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	// Включение прерывания таймера 3
	NVIC_EnableIRQ(TIM3_IRQn);

	//включем таймер
	//TIM3->CR1 |= TIM_CR1_CEN;
	TIM_Cmd(TIM3, ENABLE);

	// значение предыдущей итерации первой кнопки (PIN6, GPIOA)
	//last_state = GPIOA->IDR & GPIO_IDR_IDR6;
	last_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);

	for(;;)
	{
		//условие для кнопки 1
		//state = GPIOA->IDR & GPIO_IDR_IDR6;
		state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
		if (state != last_state)
		{
			//TIM3->CR1 &= ~TIM_CR1_CEN;
			TIM_Cmd(TIM3, DISABLE);
			if (state)
			{
				last_delay = delay;
				delay = UINT16_MAX;
			}
			//else if (TIM3->CNT >= delay)
			else if (TIM_GetCounter(TIM3) >= delay )
			{
				delay = UINT16_MAX;
			}
			//else if (TIM3->CNT >= 200 - 1)
			else if (TIM_GetCounter(TIM3) >= 200 - 1)
			{
				//delay = TIM3->CNT / 2;
				delay = TIM_GetCounter(TIM3) /2;
			}
			else
			{
				delay = last_delay;
			}
			//TIM3->CNT = 0;
			TIM_SetCounter(TIM3, 0);
			//GPIOC->ODR |= GPIO_ODR_ODR13;
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
			//TIM3 -> ARR = 2*delay-1;
			TIM_SetAutoreload(TIM3, 2*delay-1);
			//TIM3->CR1 |= TIM_CR1_CEN;
			TIM_Cmd(TIM3, ENABLE);
			last_state=state;
		}

		//условие для кнопки 2

		//if (GPIOA->IDR & GPIO_IDR_IDR8)
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
		{
			work = 1500;
		}
		//условие для кнопки 3
		//else if(!(GPIOA->IDR & GPIO_IDR_IDR3))
		else if (!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)))
		{
			 work = 2500;
		}
		//условие для кнопки 4
		//else if(!(GPIOB->IDR & GPIO_IDR_IDR12))
		else if (!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)))
		{
			work = 3500;
		}
		else
		{
			work = 500;
		}
	};
}

void TIM3_IRQHandler(void)
{
	 //TIM3->SR &= ~TIM_SR_UIF; //Clean UIF Flag
	 TIM_ClearFlag(TIM3, TIM_FLAG_Update);

		//if ( GPIOC->ODR & GPIO_ODR_ODR13 )
		if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
		{
			//TIM3 -> ARR = 2*work-1;
			TIM_SetAutoreload(TIM3, 2*work-1);
			//GPIOC->ODR &= ~ GPIO_ODR_ODR13;
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}
		else
		{
			//TIM3 -> ARR = 2*delay-1;
			TIM_SetAutoreload(TIM3, 2*delay-1);
			//GPIOC->ODR |= GPIO_ODR_ODR13;
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}

}
