/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "stm32f3xx_hal.h"
#include "tim.h"
#include "usart.h"

/* USER CODE BEGIN Includes */
#include "ds3234_spi.h"
#include "ringbuf.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern uint32_t SystemCoreClock;
static ts t;
volatile uint8_t timer7_flag = 0;
volatile uint8_t rx_flag = 0;

#define RBSIZE  512
ringbuffer_t rb; 
uint8_t rbuff[RBSIZE];
uint8_t userbuff[RBSIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
int cnv_str_to_int(int* ri, char* src, char sep);
void ds3234_settime_string(char* dtstr);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#if 1
static char stxbuff[128];
#define printf(...)                                                            \
    do {                                                                       \
        sprintf(stxbuff, __VA_ARGS__);                                         \
        HAL_UART_Transmit(&huart2, (uint8_t *)stxbuff, strlen(stxbuff), 1000); \
    } while (0)
#endif
#define CS_HI() HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_SET);
#define CS_LO() HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_RESET);

/* USER CODE END 0 */

int main(void)
{
    /* USER CODE BEGIN 1 */
    uint8_t pos = 0;
    ringbuf_init(&rb, rbuff, RBSIZE);
    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init();
    MX_TIM7_Init();

    /* USER CODE BEGIN 2 */    
    enable_uart2_rx_irq();
    ds3234_init();
    HAL_Delay(1000);
    HAL_TIM_Base_Start_IT(&htim7);
    // ds3234_set(2016, 12, 16, 0, 25, 18);

    printf("\r\n*** NUCLEO F303K8 START... ***\r\n\r\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if (timer7_flag) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);        
            ds3234_gettime(&t);

            printf("%04d-%02d-%02d %02d:%02d:%02d\r\n",
                   t.year,
                   t.mon,
                   t.mday,
                   t.hour,
                   t.min,
                   t.sec);
            printf("system core clock = %u\r\n", SystemCoreClock);

            timer7_flag = 0;
        }

        if (rx_flag) {
            uint32_t used = ringbuf_used(&rb);      // 링버퍼에 데이터가 들어있는지 조사

            if (used) {
                ringbuf_read(&rb, userbuff, used);    // 사용자 버퍼로 데이터 복사
                userbuff[used] = 0;
                /* 사용자 버퍼 처리 */
                printf("%s", userbuff);

                if ((userbuff[0] == '{') && (userbuff[21] == '}')) {                    
                    ds3234_settime_string((char*)(userbuff + 1));
                }
            }

            rx_flag = 0;
        }

        __WFE();
    }
    /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

int cnv_str_to_int(int* ri, char* src, char sep)
{
    int lindex = 0;
    char numbuf[5];

    while (src[*ri] != sep) {
        numbuf[lindex++] = src[(*ri)++];
    }

    numbuf[lindex] = 0;
    (*ri)++;

    return atoi(numbuf);
}


void ds3234_settime_string(char* dtstr)
{
    int year; 
    uint8_t mon, mday, hour, min, sec;

    int rindex = 0;
    year = cnv_str_to_int(&rindex, dtstr, '-');
    mon = cnv_str_to_int(&rindex, dtstr, '-');
    mday = cnv_str_to_int(&rindex, dtstr, ' ');
    hour = cnv_str_to_int(&rindex, dtstr, ':');
    min = cnv_str_to_int(&rindex, dtstr, ':');
    sec = cnv_str_to_int(&rindex, dtstr, ':');

#if 0
    printf("*** year = %d\r\n", year);
    printf("*** month = %d\r\n", mon);
    printf("*** day = %d\r\n", mday);
    printf("*** hour = %d\r\n", hour);
    printf("*** min = %d\r\n", min);
    printf("*** sec = %d\r\n", sec);
#endif

    ds3234_set(year, mon, mday, hour, min, sec);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1) {
    }
    /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/