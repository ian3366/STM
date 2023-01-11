/*
 * it.c
 *
 *  Created on: Dec 30, 2022
 *      Author: sscl
 */

#include"main.h"
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htimer6;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
	HAL_TIM_IRQHandler(&htimer6);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
}

