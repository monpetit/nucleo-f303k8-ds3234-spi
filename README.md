# nucleo-f303k8-ds3234-spi
 - STM32Cube SPI Communication Example
 - Sparkfun의 DS3234 Deadon RTC 모듈에 DateTime을 읽고 쓰는 예제

# 개발환경
 - Segger Embedded Studio 3.10i
 - STM32CubeMX 4.18.0
 - STM32CubeF3 Firmware Package V1.6.0 / 01-July-2016

# 하드웨어
 - STM32 Nucleo-F303k8
 - Sparkfun Deadon RTC (DS3234)

# 결선
 - NUCLEO F303K8             DS3234
 -   GND                     1 (GND)
 -   3V3                     2 (VCC)
 -   GND                     3 (SQW)
 -   A4                      4 (CLK)
 -   A5                      5 (MISO)
 -   A6                      6 (MOSI)
 -   A3                      7 (SS)

# 소프트웨어 설정
 - UART통신: UART2 (USB VCOM), Baud rate - 38400
 - SPI통신: Full Duplex, Software Slave Select, MODE 3
 - Timer7: Up mode (1초 주기 / 64 MHz HSI)
