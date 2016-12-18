#ifndef __DS3234_SPI_H__
#define __DS3234_SPI_H__

#include <stdint.h>

//Time Register bits  - Read Mode
#define secR 0x00
#define minR 0x01
#define hoR  0x02
#define dayR 0x03
#define datR 0x04
#define monR 0x05
#define yeaR 0x06
 
//Time Register bits-Write Mode
#define secW 0x80
#define minW 0x81
#define hoW  0x82
#define dayW 0x83
#define datW 0x84
#define monW 0x85
#define yeaW 0x86

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t sec;         /* seconds */
    uint8_t min;         /* minutes */
    uint8_t hour;        /* hours */
    uint8_t mday;        /* day of the month */
    uint8_t mon;         /* month */
    int year;            /* year */
    uint8_t wday;        /* day of the week */
    uint8_t yday;        /* day in the year */
    uint8_t year_s;      /* year in short notation*/
} ts;

void ds3234_init(void);
void ds3234_settime(ts* t);
void ds3234_gettime(ts* t);
void ds3234_set(int year, uint8_t mon, uint8_t mday, uint8_t hour, uint8_t min, uint8_t sec);

#ifdef __cplusplus
}
#endif

#endif /* __DS3234_SPI_H__ */

