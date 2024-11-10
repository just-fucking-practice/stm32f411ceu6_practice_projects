#include "stm32f4xx_hal.h"

/* SET UP_COUNTER TO 0 IF DOWN COUNTER IS REQUIRED */
#define UP_COUNTER 1

/*
  Global Variables
*/
TIM_HandleTypeDef TIM_InitStruct;

/*
  functions prototypes  (Only declarations are required)
*/
static void initLED(void);

int main() {
  /*
    initialize HAL Library. This step is mandatory
    for HAL libraries to work properly. It configures
    SysTick etc. for its internal configurations.
  */
  HAL_Init();

  /*
    After HAL_Init(), System is running on default HSI 
    clock (16MHz). PLL is disabled. The clock to APB1
    bus is 16Mhz (HSI clock).
  */
  
  /*
    initialize LED GPIO - Blue-LED
  */
  initLED();
  
  /* 
    Enable clock to Timer-2 
  */
  __HAL_RCC_TIM2_CLK_ENABLE();
  
  /*
    From STM32F411CEU6 datasheet, Timer2 is clocked from
    APB1 bus (16Mhz max). In default configuration
    Timer-2 is receiving 16Mhz (HSI) bus clock.
  */        
  
  /***************************************************
           Timer-2 Configuration:
  ****************************************************/
  
  /* Select Timer-2 for further configuration */
  TIM_InitStruct.Instance = TIM2;
  
  /*
    Divide the timer-2 input frequency (16Mhz)
    by a factor of 16000 (16,000,000/16,000 = 1,000 Hz)
  */
  TIM_InitStruct.Init.Prescaler = 16000 - 1;
  
  #if (UP_COUNTER)
   /* Up-Counter mode*/
   TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
  #else 
    TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_DOWN;        
  #endif

  /*
    We want the time count to be 500msec (half a second).
    As the input frequency is 1kHz so the total
    counts required for 500msec delay:
    
    total counts = 500msec * f
           = (0.5 sec) * 1,000
           = 500
  */
  TIM_InitStruct.Init.Period = 500 - 1;
    
  /*
    Finally initialize Timer-2
  */
  while (HAL_TIM_Base_Init(&TIM_InitStruct) != HAL_OK);

  /*
    Enable timer-2 IRQ interrupt
  */
  HAL_TIM_Base_Start_IT(&TIM_InitStruct);

  /* Enable interrupt at IRQ-Level */
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  
  /*
    Start the timer
  */
  HAL_TIM_Base_Start(&TIM_InitStruct);
  
  for (;;) {}
}

/*
  Timer-2 IRQ Handler
*/
void TIM2_IRQHandler(void) {
  /* Clear the interrupt flag */
  __HAL_TIM_CLEAR_IT(&TIM_InitStruct, TIM_IT_UPDATE);
  
  /* Toggle the Blue-LED */
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

/*
  configures Blue-LED (GPIOA Pin#5).
 */
static void initLED(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
    
  /* Enable clock to GPIO-A */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* Set GPIOA Pin#5 Parameters */
  GPIO_InitStruct.Pin     = GPIO_PIN_5;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_PULLUP;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
  
  /* Init GPIOA Pin#5 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);    
}
