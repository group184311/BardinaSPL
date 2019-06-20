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

void takt(void);
void button (void);
void led (void);
void timmer(void);

int main(void)
{
	takt();
	button();
	led();
	timmer();
	// значение предыдущей итерации первой кнопки (PIN6, GPIOA)
	last_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
	for(;;)
	{
		//условие для кнопки 1
		state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
		if (state != last_state)
		{
			TIM_Cmd(TIM3, DISABLE);
			if (state)
			{
				last_delay = delay;
				delay = UINT16_MAX;
			}
			else if (TIM_GetCounter(TIM3) >= delay )
			{
				delay = UINT16_MAX;
			}
			else if (TIM_GetCounter(TIM3) >= 200 - 1)
			{
				delay = TIM_GetCounter(TIM3) /2;
			}
			else
			{
				delay = last_delay;
			}
			TIM_SetCounter(TIM3, 0);
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
			TIM_SetAutoreload(TIM3, 2*delay-1);
			TIM_Cmd(TIM3, ENABLE);
			last_state=state;
		}

		//условие для кнопки 2
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
		{
			work = 1500;
		}
		//условие для кнопки 3
		else if (!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)))
		{
			 work = 2500;
		}
		//условие для кнопки 4
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

void takt(void)
{
	//Включаем тактирование порта A, B и C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//включаем тактирование таймера
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void button(void){
	//кнопки
	GPIO_InitTypeDef BUTTON;
	//кнопка 1 - порт A, пин6 (low - CRL)
	//сброс состояния
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	BUTTON.GPIO_Pin = GPIO_Pin_6;
	BUTTON.GPIO_Mode = GPIO_Mode_IPD; //к земле
	GPIO_Init(GPIOA, &BUTTON);

	//кнопка 2 - порт A, пин8 (high - CRH)
	//сброс состояния
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	BUTTON.GPIO_Pin = GPIO_Pin_8;
	BUTTON.GPIO_Mode = GPIO_Mode_IPD; //к земле
	GPIO_Init(GPIOA, &BUTTON);

	//кнопка 3 - порт A, пин3 (low - CRL)
	//сброс состояния
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	BUTTON.GPIO_Pin = GPIO_Pin_3;
	BUTTON.GPIO_Mode = GPIO_Mode_IPU; //к питанию
	GPIO_Init(GPIOA, &BUTTON);

	//кнопка 4 - порт B, пин12 (high - CRH)
	//сброс состояния
	//вывод кнопки как вход с подтяжкой MODE=00: Input; CNF=10: Floating input (reset state)
	BUTTON.GPIO_Pin = GPIO_Pin_12;
	BUTTON.GPIO_Mode = GPIO_Mode_IPU; //к питанию
	GPIO_Init(GPIOB, &BUTTON);
}

void led(void){
	//светодиод - порт C, пин13 (high - CRH)
	GPIO_InitTypeDef LED;
	//сброс состояния
	//вывод светодиода как выход pull/push MODE=10: Max. output speed 2 MHz ; CRH=00:  General purpose output push-pull
	LED.GPIO_Pin = GPIO_Pin_13;
	LED.GPIO_Speed =  GPIO_Speed_2MHz;
	LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &LED);
	//обнулить загорание
	GPIO_WriteBit(GPIOC, LED.GPIO_Pin, Bit_SET);
}

void timmer(void)
{
	//запускаем таймер на тактовой частоте в 1000 Hz
	//Fmax = 36 000 000 Hz, нужно 1000 => 36 0000 000 / 1 000 = 36 000
	//задаем предделитель TIMx_PSC
	//начальное значение периода - 500 тактов (delay)
	TIM_TimeBaseInitTypeDef TIM3_CLOCK;
	TIM3_CLOCK.TIM_Prescaler = 36000 - 1;
	TIM3_CLOCK.TIM_Period = 2*delay-1;
	TIM3_CLOCK.TIM_CounterMode = TIM_CounterMode_Up;
	TIM3_CLOCK.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM3_CLOCK);

	//разрешаем прерывания таймера 3 по переполнению (DMA/interrupt enable register ;  Update interrupt enable)
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	// Включение прерывания таймера 3
	NVIC_EnableIRQ(TIM3_IRQn);

	//включем таймер
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	 TIM_ClearFlag(TIM3, TIM_FLAG_Update);

		if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
		{
			TIM_SetAutoreload(TIM3, 2*work-1);
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}
		else
		{
			TIM_SetAutoreload(TIM3, 2*delay-1);
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}
}
