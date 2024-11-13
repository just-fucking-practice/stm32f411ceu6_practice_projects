/* @description:    This code configures Timer-4 output
 *                  channels, connected to onboard GPIOS
 *                  in ouput compare mode. Each Channel
 *                  upon successful match, toggle one of
 *                  the onboard LED. */

#include "stm32f4xx_hal.h"

#define UP_COUNTER 1

static void initLEDs(void);
static void initTimer(void);

TIM_HandleTypeDef TIM_InitStruct;

int main()
{
  HAL_Init();

  NVIC_SetPriorityGrouping(0);

  initLEDs();
  initTimer();

  for (;;)
  {
  }
}

/*
  configures onboard LEDs (GPIOB Pin#6, GPIOB Pin#7, GPIOB Pin#8, GPIOB Pin#9).
 */
static void initLEDs(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void initTimer(void)
{
  TIM_OC_InitTypeDef TIM_Channel_InitStruct;

  __HAL_RCC_TIM4_CLK_ENABLE();

  TIM_InitStruct.Instance = TIM4;
  TIM_InitStruct.Init.Prescaler = 1000;

#if (UP_COUNTER)
  TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
#else
  TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_DOWN;
#endif

TIM_InitStruct.Init.Period = 8000;

while(HAL_TIM_Base_Init(&TIM_InitStruct) != HAL_OK);

TIM_Channel_InitStruct.OCMode = TIM_OCMODE_TOGGLE;
TIM_Channel_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
TIM_Channel_InitStruct.OCFastMode = TIM_OCFAST_DISABLE;

TIM_Channel_InitStruct.Pulse = 2000;
HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_1);

TIM_Channel_InitStruct.Pulse = 4000;
HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_2);

TIM_Channel_InitStruct.Pulse = 6000;
HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_3);

TIM_Channel_InitStruct.Pulse = 8000;
HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_4);

HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_1);
HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_2);
HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_3);
HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_4);
}
