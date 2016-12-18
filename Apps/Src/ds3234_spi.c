#include "gpio.h"
#include "spi.h"
#include "ds3234_spi.h"
#include "stm32f3xx_hal.h"

#define CS_HI() HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_SET);
#define CS_LO() HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_RESET);

__STATIC_INLINE void spi_write(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
}

__STATIC_INLINE uint8_t spi_read(void)
{
    uint8_t n;
    HAL_SPI_Receive(&hspi1, &n, 1, 1000);
    return n;
}

__STATIC_INLINE uint8_t dec2bcd(const uint8_t val)
{
    return ((val / 10 * 16) + (val % 10));
}

__STATIC_INLINE  uint8_t bcd2dec(const uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}


void ds3234_init(void)
{
    static uint8_t txbuff[2] = {0x8e, 0x60};

    // Chip must be deselected
    CS_HI(); // cs = 1;

    MX_SPI1_Init();

    // Select the device by seting chip select low
    CS_LO(); // cs = 0;

    // Send 0x8f, the command to read the WHOAMI register
    HAL_SPI_Transmit(&hspi1, txbuff, sizeof(txbuff), 1000);

    // Deselect the device
    CS_HI(); // cs = 1;
}


void ds3234_settime(ts* t)
{
    uint8_t i, century, txdata;

    if (t->year > 2000) {
        century = 0x80;
        t->year_s = t->year - 2000;
    }
    else {
        century = 0;
        t->year_s = t->year - 1900;
    }

    uint8_t TimeDate[7] = { t->sec, t->min, t->hour, t->wday, t->mday, t->mon, t->year_s };
    for (i = 0; i <= 6; i++) {
        CS_LO(); // cs = 0;
        spi_write(i + 0x80);
        if (i == 5)
            spi_write(dec2bcd(TimeDate[5]) + century);
        else
            spi_write(dec2bcd(TimeDate[i]));
        CS_HI(); // cs = 1;
    }
}


void ds3234_gettime(ts* t)
{
    uint8_t TimeDate[7];        //second,minute,hour,dow,day,month,year
    uint8_t century = 0;
    uint8_t i, n;
    uint16_t year_full;

    for (i = 0; i <= 6; i++) {
        CS_LO(); // cs = 0;
        spi_write(i + 0x00);
        spi_write(0x00);
        n = spi_read();
        CS_HI(); // cs = 1;

        if (i == 5) {           // month address also contains the century on bit7
            TimeDate[5] = bcd2dec(n & 0x1F);
            century = (n & 0x80) >> 7;
        }
        else {
            TimeDate[i] = bcd2dec(n);
        }
    }

    if (century == 1)
        year_full = 2000 + TimeDate[6];
    else
        year_full = 1900 + TimeDate[6];

    t->sec = TimeDate[0];
    t->min = TimeDate[1];
    t->hour = TimeDate[2];
    t->mday = TimeDate[4];
    t->mon = TimeDate[5];
    t->year = year_full;
    t->wday = TimeDate[3];
    t->year_s = TimeDate[6];
}

void ds3234_set(int year, uint8_t mon, uint8_t mday, uint8_t hour, uint8_t min, uint8_t sec)
{
    static ts t;

    t.year = year;
    t.mon = mon;
    t.mday = mday,
      t.hour = hour;
    t.min = min;
    t.sec = sec;
    ds3234_settime(&t);
}
