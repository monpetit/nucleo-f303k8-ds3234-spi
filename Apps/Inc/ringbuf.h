//*****************************************************************************
//
// ringbuf.h - Defines and Macros for the ring buffer utilities.
//
// Copyright (c) 2008-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 1.1 of the Tiva Utility Library.
//
//*****************************************************************************

#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdbool.h>
#include <stdint.h>

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// The structure used for encapsulating all the items associated with a
// ring buffer.
//
//*****************************************************************************
typedef struct __ringbuffer_t {
    //
    // The ring buffer size.
    //
    uint32_t ui32Size;

    //
    // The ring buffer write index.
    //
    volatile uint32_t ui32WriteIndex;

    //
    // The ring buffer read index.
    //
    volatile uint32_t ui32ReadIndex;

    //
    // The ring buffer.
    //
    uint8_t *pui8Buf;

}
ringbuffer_t;

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern bool ringbuf_full(ringbuffer_t *psRingBuf);
extern bool ringbuf_empty(ringbuffer_t *psRingBuf);
extern void ringbuf_flush(ringbuffer_t *psRingBuf);
extern uint32_t ringbuf_used(ringbuffer_t *psRingBuf);
extern uint32_t ringbuf_free(ringbuffer_t *psRingBuf);
extern uint32_t ringbuf_contig_used(ringbuffer_t *psRingBuf);
extern uint32_t ringbuf_contig_free(ringbuffer_t *psRingBuf);
extern uint32_t ringbuf_size(ringbuffer_t *psRingBuf);
extern uint8_t ringbuf_readone(ringbuffer_t *psRingBuf);
extern void ringbuf_read(ringbuffer_t *psRingBuf, uint8_t *pui8Data,
                         uint32_t ui32Length);
extern void ringbuf_writeone(ringbuffer_t *psRingBuf, uint8_t ui8Data);
extern void ringbuf_write(ringbuffer_t *psRingBuf, uint8_t *pui8Data,
                          uint32_t ui32Length);
extern void ringbuf_advwrite(ringbuffer_t *psRingBuf,
                             uint32_t ui32NumBytes);
extern void ringbuf_advread(ringbuffer_t *psRingBuf,
                            uint32_t ui32NumBytes);
extern void ringbuf_init(ringbuffer_t *psRingBuf, uint8_t *pui8Buf,
                         uint32_t ui32Size);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __RINGBUF_H__
