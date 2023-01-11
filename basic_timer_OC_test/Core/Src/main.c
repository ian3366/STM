/*
 * main.c
 *
 *  Created on: Dec 27, 2022
 *      Author: sscl
 */


#include "main.h"

void Error_handler();
void uart3_init();
void gpio_init();
void SystemClock_Config_HSE(uint8_t clock_freq);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
void timer2_init();

UART_HandleTypeDef huart3;
TIM_HandleTypeDef htimer2;

uint32_t pulse1_value = 25000;	// 500Hz
uint32_t pulse2_value = 12500;	// 1000Hz
uint32_t pulse3_value = 6250;	// 2000Hz
uint32_t pulse4_value = 3125;	// 4000Hz
uint32_t ccr_content;


char* data = "test";


int main(void)
{
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	gpio_init();
	uart3_init();
	timer2_init();

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}

	if( HAL_TIM_OC_Start_IT(&htimer2,TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}

	while(1);

	uint16_t datalen = strlen(data);
	while(1)
	{

		HAL_UART_Transmit(&huart3,(uint8_t*)data,datalen, HAL_MAX_DELAY);
		HAL_Delay(1000);
	}


	return 0;
}

void gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_0;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);
}


void timer2_init()
{
	TIM_OC_InitTypeDef tim2oc;

	htimer2.Instance =TIM2;
	htimer2.Init.Period = 0XFFFFFFFF;
	htimer2.Init.Prescaler = 1;
	if (HAL_TIM_OC_Init(&htimer2))
	{
		Error_handler();
	}

	tim2oc.OCMode  = TIM_OCMODE_TOGGLE;
	tim2oc.OCNPolarity =  TIM_OCPOLARITY_HIGH;
	tim2oc.Pulse = pulse1_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2oc, TIM_CHANNEL_1)!= HAL_OK)
	{
		Error_handler();
	}
	tim2oc.Pulse = pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2oc, TIM_CHANNEL_2)!= HAL_OK)
	{
		Error_handler();
	}
	tim2oc.Pulse = pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2oc, TIM_CHANNEL_3)!= HAL_OK)
	{
		Error_handler();
	}
	tim2oc.Pulse = pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2oc, TIM_CHANNEL_4)!= HAL_OK)
	{
		Error_handler();
	}





}

void uart3_init()
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate=115200;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity=UART_PARITY_NONE;
	huart3.Init.WordLength =UART_WORDLENGTH_8B;
	if(!(HAL_UART_Init(&huart3)==HAL_OK))
	{
		Error_handler();
	}
}

void SystemClock_Config_HSE(uint8_t clock_freq)

{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;
    uint8_t flash_latency=0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSI ;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 2;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
          flash_latency = 3;
	     break;

	  default:
	   return ;
	 }

		if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
			Error_handler();
	}



	if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
	{
		Error_handler();
	}


	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);



 }
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
 {
   /* TIM3_CH1 toggling with frequency = 500 Hz */
   if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
   {
	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
   }

   /* TIM3_CH2 toggling with frequency = 1000 Hz */
   if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
   {
	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);

   }

   /* TIM3_CH3 toggling with frequency = 2000 Hz */
   if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
   {
	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);

   }

   /* TIM3_CH4 toggling with frequency = 4000 Hz */
   if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
   {
	    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);

   }
 }


void Error_handler()
{
	while(1);
}
