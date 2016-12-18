/*****************************************************************************
 *                   SEGGER Microcontroller GmbH & Co. KG                    *
 *            Solutions for real time microcontroller applications           *
 *****************************************************************************
 *                                                                           *
 *               (c) 2015 SEGGER Microcontroller GmbH & Co. KG               *
 *                                                                           *
 *           Internet: www.segger.com   Support: support@segger.com          *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 *                         Preprocessor Definitions                          *
 *                         ------------------------                          *
 * VECTORS_IN_RAM                                                            *
 *                                                                           *
 *   If defined, an area of RAM will large enough to store the vector table  *
 *   will be reserved.                                                       *
 *                                                                           *
 *****************************************************************************/

  .syntax unified
  .code 16

  .section .init, "ax"
  .align 0

/*****************************************************************************
 * Default Exception Handlers                                                *
 *****************************************************************************/

  .thumb_func
  .weak NMI_Handler
NMI_Handler:
  b .

  .thumb_func
  .weak HardFault_Handler
HardFault_Handler:
  b .

  .thumb_func
  .weak	MemManage_Handler
MemManage_Handler:
  b .

  .thumb_func
  .weak	BusFault_Handler
BusFault_Handler:
  b .

  .thumb_func
  .weak	UsageFault_Handler
UsageFault_Handler:
  b .

  .thumb_func
  .weak SVC_Handler
SVC_Handler:
  b .

  .thumb_func
  .weak DebugMon_Handler
DebugMon_Handler:
  b .

  .thumb_func
  .weak PendSV_Handler
PendSV_Handler:
  b .

  .thumb_func
  .weak SysTick_Handler
SysTick_Handler:
  b .

  .thumb_func
Dummy_Handler:
  b .

#if defined(__OPTIMIZATION_SMALL)

  .weak	WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Dummy_Handler

  .weak	PVD_IRQHandler
  .thumb_set PVD_IRQHandler,Dummy_Handler

  .weak	TAMP_STAMP_IRQHandler
  .thumb_set TAMP_STAMP_IRQHandler,Dummy_Handler

  .weak	RTC_WKUP_IRQHandler
  .thumb_set RTC_WKUP_IRQHandler,Dummy_Handler

  .weak	FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Dummy_Handler

  .weak	RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Dummy_Handler

  .weak	EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Dummy_Handler

  .weak	EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Dummy_Handler

  .weak	EXTI2_TSC_IRQHandler
  .thumb_set EXTI2_TSC_IRQHandler,Dummy_Handler

  .weak	EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Dummy_Handler

  .weak	EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel2_IRQHandler
  .thumb_set DMA1_Channel2_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel3_IRQHandler
  .thumb_set DMA1_Channel3_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel4_IRQHandler
  .thumb_set DMA1_Channel4_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel5_IRQHandler
  .thumb_set DMA1_Channel5_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel6_IRQHandler
  .thumb_set DMA1_Channel6_IRQHandler,Dummy_Handler

  .weak	DMA1_Channel7_IRQHandler
  .thumb_set DMA1_Channel7_IRQHandler,Dummy_Handler

  .weak	ADC1_2_IRQHandler
  .thumb_set ADC1_2_IRQHandler,Dummy_Handler

  .weak	CAN_TX_IRQHandler
  .thumb_set CAN_TX_IRQHandler,Dummy_Handler

  .weak	CAN_RX0_IRQHandler
  .thumb_set CAN_RX0_IRQHandler,Dummy_Handler

  .weak	CAN_RX1_IRQHandler
  .thumb_set CAN_RX1_IRQHandler,Dummy_Handler

  .weak	CAN_SCE_IRQHandler
  .thumb_set CAN_SCE_IRQHandler,Dummy_Handler

  .weak	EXTI9_5_IRQHandler
  .thumb_set EXTI9_5_IRQHandler,Dummy_Handler

  .weak	TIM1_BRK_TIM15_IRQHandler
  .thumb_set TIM1_BRK_TIM15_IRQHandler,Dummy_Handler

  .weak	TIM1_UP_TIM16_IRQHandler
  .thumb_set TIM1_UP_TIM16_IRQHandler,Dummy_Handler

  .weak	TIM1_TRG_COM_TIM17_IRQHandler
  .thumb_set TIM1_TRG_COM_TIM17_IRQHandler,Dummy_Handler

  .weak	TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Dummy_Handler

  .weak	TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Dummy_Handler

  .weak	TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Dummy_Handler

  .weak	I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Dummy_Handler

  .weak	I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Dummy_Handler

  .weak	SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Dummy_Handler

  .weak	USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Dummy_Handler

  .weak	USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Dummy_Handler

  .weak	USART3_IRQHandler
  .thumb_set USART3_IRQHandler,Dummy_Handler

  .weak	EXTI15_10_IRQHandler
  .thumb_set EXTI15_10_IRQHandler,Dummy_Handler

  .weak	RTC_Alarm_IRQHandler
  .thumb_set RTC_Alarm_IRQHandler,Dummy_Handler

  .weak	TIM6_DAC1_IRQHandler
  .thumb_set TIM6_DAC1_IRQHandler,Dummy_Handler

  .weak	TIM7_DAC2_IRQHandler
  .thumb_set TIM7_DAC2_IRQHandler,Dummy_Handler
  
  .weak	COMP2_IRQHandler
  .thumb_set COMP2_IRQHandler,Dummy_Handler
  
  .weak	COMP4_6_IRQHandler
  .thumb_set COMP4_6_IRQHandler,Dummy_Handler
  
  .weak	FPU_IRQHandler
  .thumb_set FPU_IRQHandler,Dummy_Handler
#else

  .thumb_func
  .weak WWDG_IRQHandler
WWDG_IRQHandler:
  b .

  .thumb_func
  .weak PVD_IRQHandler
PVD_IRQHandler:
  b .

  .thumb_func
  .weak TAMP_STAMP_IRQHandler
TAMP_STAMP_IRQHandler:
  b .

  .thumb_func
  .weak RTC_WKUP_IRQHandler
RTC_WKUP_IRQHandler:
  b .

  .thumb_func
  .weak FLASH_IRQHandler
FLASH_IRQHandler:
  b .

  .thumb_func
  .weak RCC_IRQHandler
RCC_IRQHandler:
  b .

  .thumb_func
  .weak EXTI0_IRQHandler
EXTI0_IRQHandler:
  b .

  .thumb_func
  .weak EXTI1_IRQHandler
EXTI1_IRQHandler:
  b .

  .thumb_func
  .weak EXTI2_TSC_IRQHandler
EXTI2_TSC_IRQHandler:
  b .

  .thumb_func
  .weak EXTI3_IRQHandler
EXTI3_IRQHandler:
  b .

  .thumb_func
  .weak EXTI4_IRQHandler
EXTI4_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel1_IRQHandler
DMA1_Channel1_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel2_IRQHandler
DMA1_Channel2_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel3_IRQHandler
DMA1_Channel3_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel4_IRQHandler
DMA1_Channel4_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel5_IRQHandler
DMA1_Channel5_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel6_IRQHandler
DMA1_Channel6_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_Channel7_IRQHandler
DMA1_Channel7_IRQHandler:
  b .

  .thumb_func
  .weak ADC1_2_IRQHandler
ADC1_2_IRQHandler:
  b .

  .thumb_func
  .weak CAN_TX_IRQHandler
CAN_TX_IRQHandler:
  b .

  .thumb_func
  .weak CAN_RX0_IRQHandler
CAN_RX0_IRQHandler:
  b .

  .thumb_func
  .weak CAN_RX1_IRQHandler
CAN_RX1_IRQHandler:
  b .

  .thumb_func
  .weak CAN_SCE_IRQHandler
CAN_SCE_IRQHandler:
  b .

  .thumb_func
  .weak EXTI9_5_IRQHandler
EXTI9_5_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_BRK_TIM15_IRQHandler
TIM1_BRK_TIM15_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_UP_TIM16_IRQHandler
TIM1_UP_TIM16_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_TRG_COM_TIM17_IRQHandler
TIM1_TRG_COM_TIM17_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_CC_IRQHandler
TIM1_CC_IRQHandler:
  b .

  .thumb_func
  .weak TIM2_IRQHandler
TIM2_IRQHandler:
  b .

  .thumb_func
  .weak TIM3_IRQHandler
TIM3_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_EV_IRQHandler
I2C1_EV_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_ER_IRQHandler
I2C1_ER_IRQHandler:
  b .

  .thumb_func
  .weak SPI1_IRQHandler
SPI1_IRQHandler:
  b .

  .thumb_func
  .weak USART1_IRQHandler
USART1_IRQHandler:
  b .

  .thumb_func
  .weak USART2_IRQHandler
USART2_IRQHandler:
  b .

  .thumb_func
  .weak USART3_IRQHandler
USART3_IRQHandler:
  b .

  .thumb_func
  .weak EXTI15_10_IRQHandler
EXTI15_10_IRQHandler:
  b .

  .thumb_func
  .weak RTC_Alarm_IRQHandler
RTC_Alarm_IRQHandler:
  b .

  .thumb_func
  .weak TIM6_DAC1_IRQHandler
TIM6_DAC1_IRQHandler:
  b .

  .thumb_func
  .weak TIM7_DAC2_IRQHandler
TIM7_DAC2_IRQHandler:
  b .

  .thumb_func
  .weak COMP2_IRQHandler
COMP2_IRQHandler:
  b .

  .thumb_func
  .weak COMP4_6_IRQHandler
COMP4_6_IRQHandler:
  b .

  .thumb_func
  .weak FPU_IRQHandler
FPU_IRQHandler:
  b .

#endif

/*****************************************************************************
 * Vector Table                                                              *
 *****************************************************************************/

  .section .vectors, "ax"
  .align 0
  .global _vectors
  .extern __stack_end__
  .extern Reset_Handler

_vectors:
  .word __stack_end__
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word MemManage_Handler
  .word BusFault_Handler
  .word UsageFault_Handler
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word SVC_Handler
  .word DebugMon_Handler
  .word 0    /* Reserved */
  .word PendSV_Handler
  .word SysTick_Handler
  .word WWDG_IRQHandler
  .word PVD_IRQHandler
  .word TAMP_STAMP_IRQHandler
  .word RTC_WKUP_IRQHandler
  .word FLASH_IRQHandler
  .word RCC_IRQHandler
  .word EXTI0_IRQHandler
  .word EXTI1_IRQHandler
  .word EXTI2_TSC_IRQHandler
  .word EXTI3_IRQHandler
  .word EXTI4_IRQHandler
  .word DMA1_Channel1_IRQHandler
  .word DMA1_Channel2_IRQHandler
  .word DMA1_Channel3_IRQHandler
  .word DMA1_Channel4_IRQHandler
  .word DMA1_Channel5_IRQHandler
  .word DMA1_Channel6_IRQHandler
  .word DMA1_Channel7_IRQHandler
  .word ADC1_2_IRQHandler
  .word CAN_TX_IRQHandler
  .word CAN_RX0_IRQHandler
  .word CAN_RX1_IRQHandler
  .word CAN_SCE_IRQHandler
  .word EXTI9_5_IRQHandler
  .word TIM1_BRK_TIM15_IRQHandler
  .word TIM1_UP_TIM16_IRQHandler
  .word TIM1_TRG_COM_TIM17_IRQHandler
  .word TIM1_CC_IRQHandler
  .word TIM2_IRQHandler
  .word TIM3_IRQHandler
  .word 0    /* Reserved */
  .word I2C1_EV_IRQHandler
  .word I2C1_ER_IRQHandler
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word SPI1_IRQHandler
  .word 0    /* Reserved */
  .word USART1_IRQHandler
  .word USART2_IRQHandler
  .word USART3_IRQHandler
  .word EXTI15_10_IRQHandler
  .word RTC_Alarm_IRQHandler
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word TIM6_DAC1_IRQHandler
  .word TIM7_DAC2_IRQHandler
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word COMP2_IRQHandler
  .word COMP4_6_IRQHandler
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word 0    /* Reserved */
  .word FPU_IRQHandler
_vectors_end:

#ifdef VECTORS_IN_RAM
  .section .vectors_ram, "ax"
  .align 0
  .global _vectors_ram

_vectors_ram:
  .space _vectors_end - _vectors, 0
#endif
