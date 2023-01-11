/*
 * msp.c
 *
 *  Created on: Dec 30, 2022
 *      Author: sscl
 */
#include "main.h"

void HAL_MspInit(void)
{
 //Here will do low level processor specific inits.
	//1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions
	// cortex manual-> system control block
	//3. configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
	//void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	 GPIO_InitTypeDef gpio_uart;
	 //here we are going to do the low level inits. of the USART2 peripheral

	 //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral
	 __HAL_RCC_USART3_CLK_ENABLE();
	 __HAL_RCC_USART3_CLK_SLEEP_DISABLE();
	 __HAL_RCC_GPIOD_CLK_ENABLE();

	 //2 . Do the pin muxing configurations
	 gpio_uart.Pin = GPIO_PIN_8;
	 gpio_uart.Mode =GPIO_MODE_AF_PP;
	 gpio_uart.Pull = GPIO_NOPULL;
	 gpio_uart.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 gpio_uart.Alternate =  GPIO_AF7_USART3; //UART2_TX
	 HAL_GPIO_Init(GPIOD,&gpio_uart);

	 gpio_uart.Pin = GPIO_PIN_9; //UART2_RX
	 HAL_GPIO_Init(GPIOD,&gpio_uart);
	 //3 . Enable the IRQ and set up the priority (NVIC settings )
	 HAL_NVIC_EnableIRQ(USART3_IRQn);
	 HAL_NVIC_SetPriority(USART3_IRQn,15,0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	//2. Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. setup the priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);

}

