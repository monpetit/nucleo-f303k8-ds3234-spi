//*****************************************************************************
//
// ringbuf.c - Ring buffer management utilities.
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

#include "stm32f3xx.h"
#include <stdio.h>
#include "ringbuf.h"

//*****************************************************************************
//
//! \addtogroup ringbuf_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Define NULL, if not already defined.
//
//*****************************************************************************
#ifndef NULL
#define NULL                    ((void *)0)
#endif

#define __USE_INTERRUPT         1
#define     INT_Enable()        __enable_irq()
#define     INT_Disable()       __disable_irq()

/*
extern void blink_forever(void);
inline void __ASSERT(bool exp)
{
    if (!(exp)) {
        puts("ASSERTION ERROR");
        blink_forever();
    }
}
*/
#define __ASSERT(EXP)

//*****************************************************************************
//
// Change the value of a variable atomically.
//
// \param pui32Val points to the index whose value is to be modified.
// \param ui32Delta is the number of bytes to increment the index by.
// \param ui32Size is the size of the buffer the index refers to.
//
// This function is used to increment a read or write buffer index that may be
// written in various different contexts.  It ensures that the
// read/modify/write sequence is not interrupted and, hence, guards against
// corruption of the variable.  The new value is adjusted for buffer wrap.
//
// \return None.
//
//*****************************************************************************
static void
update_index_atomic(uint32_t volatile *pui32Val, uint32_t ui32Delta,
                    uint32_t ui32Size)
{
    //
    // Turn interrupts off temporarily.
    //
#if __USE_INTERRUPT
    INT_Disable();
#endif

    //
    // Update the variable value.
    //
    *pui32Val += ui32Delta;

    //
    // Correct for wrap.  We use a loop here since we don't want to use a
    // modulus operation with interrupts off but we don't want to fail in
    // case ui32Delta is greater than ui32Size (which is extremely unlikely
    // but...)
    //
    while (*pui32Val >= ui32Size) {
        *pui32Val -= ui32Size;
    }

    //
    // Restore the interrupt state
    //
#if __USE_INTERRUPT
    INT_Enable();
#endif
}

//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is full or not.
//!
//! \param psRingBuf is the ring buffer object to empty.
//!
//! This function is used to determine whether or not a given ring buffer is
//! full.  The structure is specifically to ensure that we do not see
//! warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is full or \b false otherwise.
//
//*****************************************************************************
bool
ringbuf_full(ringbuffer_t *psRingBuf)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ui32Write = psRingBuf->ui32WriteIndex;
    ui32Read = psRingBuf->ui32ReadIndex;

    //
    // Return the full status of the buffer.
    //
    return ((((ui32Write + 1) % psRingBuf->ui32Size) == ui32Read) ? true :
            false);
}

//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is empty or not.
//!
//! \param psRingBuf is the ring buffer object to empty.
//!
//! This function is used to determine whether or not a given ring buffer is
//! empty.  The structure is specifically to ensure that we do not see
//! warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is empty or \b false otherwise.
//
//*****************************************************************************
bool
ringbuf_empty(ringbuffer_t *psRingBuf)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ui32Write = psRingBuf->ui32WriteIndex;
    ui32Read = psRingBuf->ui32ReadIndex;

    //
    // Return the empty status of the buffer.
    //
    return ((ui32Write == ui32Read) ? true : false);
}

//*****************************************************************************
//
//! Empties the ring buffer.
//!
//! \param psRingBuf is the ring buffer object to empty.
//!
//! Discards all data from the ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_flush(ringbuffer_t *psRingBuf)
{
    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Set the Read/Write pointers to be the same.  Do this with interrupts
    // disabled to prevent the possibility of corruption of the read index.
    //
#if __USE_INTERRUPT
    INT_Disable();
#endif
    psRingBuf->ui32ReadIndex = psRingBuf->ui32WriteIndex;
#if __USE_INTERRUPT
    INT_Enable();
#endif
}

//*****************************************************************************
//
//! Returns number of bytes stored in ring buffer.
//!
//! \param psRingBuf is the ring buffer object to check.
//!
//! This function returns the number of bytes stored in the ring buffer.
//!
//! \return Returns the number of bytes stored in the ring buffer.
//
//*****************************************************************************
uint32_t
ringbuf_used(ringbuffer_t *psRingBuf)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ui32Write = psRingBuf->ui32WriteIndex;
    ui32Read = psRingBuf->ui32ReadIndex;

    //
    // Return the number of bytes contained in the ring buffer.
    //
    return ((ui32Write >= ui32Read) ? (ui32Write - ui32Read) :
            (psRingBuf->ui32Size - (ui32Read - ui32Write)));
}

//*****************************************************************************
//
//! Returns number of bytes available in a ring buffer.
//!
//! \param psRingBuf is the ring buffer object to check.
//!
//! This function returns the number of bytes available in the ring buffer.
//!
//! \return Returns the number of bytes available in the ring buffer.
//
//*****************************************************************************
uint32_t
ringbuf_free(ringbuffer_t *psRingBuf)
{
    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Return the number of bytes available in the ring buffer.
    //
    return ((psRingBuf->ui32Size - 1) - ringbuf_used(psRingBuf));
}

//*****************************************************************************
//
//! Returns number of contiguous bytes of data stored in ring buffer ahead of
//! the current read pointer.
//!
//! \param psRingBuf is the ring buffer object to check.
//!
//! This function returns the number of contiguous bytes of data available in
//! the ring buffer ahead of the current read pointer.  This represents the
//! largest block of data which does not straddle the buffer wrap.
//!
//! \return Returns the number of contiguous bytes available.
//
//*****************************************************************************
uint32_t
ringbuf_contig_used(ringbuffer_t *psRingBuf)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ui32Write = psRingBuf->ui32WriteIndex;
    ui32Read = psRingBuf->ui32ReadIndex;

    //
    // Return the number of contiguous bytes available.
    //
    return ((ui32Write >= ui32Read) ? (ui32Write - ui32Read) :
            (psRingBuf->ui32Size - ui32Read));
}

//*****************************************************************************
//
//! Returns number of contiguous free bytes available in a ring buffer.
//!
//! \param psRingBuf is the ring buffer object to check.
//!
//! This function returns the number of contiguous free bytes ahead of the
//! current write pointer in the ring buffer.
//!
//! \return Returns the number of contiguous bytes available in the ring
//! buffer.
//
//*****************************************************************************
uint32_t
ringbuf_contig_free(ringbuffer_t *psRingBuf)
{
    uint32_t ui32Write;
    uint32_t ui32Read;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ui32Write = psRingBuf->ui32WriteIndex;
    ui32Read = psRingBuf->ui32ReadIndex;

    //
    // Return the number of contiguous bytes available.
    //
    if (ui32Read > ui32Write) {
        //
        // The read pointer is above the write pointer so the amount of free
        // space is the difference between the two indices minus 1 to account
        // for the buffer full condition (write index one behind read index).
        //
        return ((ui32Read - ui32Write) - 1);
    }
    else {
        //
        // If the write pointer is above the read pointer, the amount of free
        // space is the size of the buffer minus the write index.  We need to
        // add a special-case adjustment if the read index is 0 since we need
        // to leave 1 byte empty to ensure we can tell the difference between
        // the buffer being full and empty.
        //
        return (psRingBuf->ui32Size - ui32Write - ((ui32Read == 0) ? 1 : 0));
    }
}

//*****************************************************************************
//
//! Return size in bytes of a ring buffer.
//!
//! \param psRingBuf is the ring buffer object to check.
//!
//! This function returns the size of the ring buffer.
//!
//! \return Returns the size in bytes of the ring buffer.
//
//*****************************************************************************
uint32_t
ringbuf_size(ringbuffer_t *psRingBuf)
{
    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Return the number of bytes available in the ring buffer.
    //
    return (psRingBuf->ui32Size);
}

//*****************************************************************************
//
//! Reads a single byte of data from a ring buffer.
//!
//! \param psRingBuf points to the ring buffer to be written to.
//!
//! This function reads a single byte of data from a ring buffer.
//!
//! \return The byte read from the ring buffer.
//
//*****************************************************************************
uint8_t
ringbuf_readone(ringbuffer_t *psRingBuf)
{
    uint8_t ui8Temp;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Verify that space is available in the buffer.
    //
    __ASSERT(ringbuf_used(psRingBuf) != 0);

    //
    // Write the data byte.
    //
    ui8Temp = psRingBuf->pui8Buf[psRingBuf->ui32ReadIndex];

    //
    // Increment the read index.
    //
    update_index_atomic(&psRingBuf->ui32ReadIndex, 1, psRingBuf->ui32Size);

    //
    // Return the character read.
    //
    return (ui8Temp);
}

//*****************************************************************************
//
//! Reads data from a ring buffer.
//!
//! \param psRingBuf points to the ring buffer to be read from.
//! \param pui8Data points to where the data should be stored.
//! \param ui32Length is the number of bytes to be read.
//!
//! This function reads a sequence of bytes from a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_read(ringbuffer_t *psRingBuf, uint8_t *pui8Data, uint32_t ui32Length)
{
    uint32_t ui32Temp;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);
    __ASSERT(pui8Data != NULL);
    __ASSERT(ui32Length != 0);

    //
    // Verify that data is available in the buffer.
    //
    __ASSERT(ui32Length <= ringbuf_used(psRingBuf));

    //
    // Read the data from the ring buffer.
    //
    for (ui32Temp = 0; ui32Temp < ui32Length; ui32Temp++) {
        pui8Data[ui32Temp] = ringbuf_readone(psRingBuf);
    }
}

//*****************************************************************************
//
//! Remove bytes from the ring buffer by advancing the read index.
//!
//! \param psRingBuf points to the ring buffer from which bytes are to be
//! removed.
//! \param ui32NumBytes is the number of bytes to be removed from the buffer.
//!
//! This function advances the ring buffer read index by a given number of
//! bytes, removing that number of bytes of data from the buffer.  If
//! \e ui32NumBytes is larger than the number of bytes currently in the buffer,
//! the buffer is emptied.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_advread(ringbuffer_t *psRingBuf, uint32_t ui32NumBytes)
{
    uint32_t ui32Count;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Make sure that we are not being asked to remove more data than is
    // there to be removed.
    //
    ui32Count = ringbuf_used(psRingBuf);
    ui32Count = (ui32Count < ui32NumBytes) ? ui32Count : ui32NumBytes;

    //
    // Advance the buffer read index by the required number of bytes.
    //
    update_index_atomic(&psRingBuf->ui32ReadIndex, ui32Count,
                        psRingBuf->ui32Size);
}

//*****************************************************************************
//
//! Add bytes to the ring buffer by advancing the write index.
//!
//! \param psRingBuf points to the ring buffer to which bytes have been added.
//! \param ui32NumBytes is the number of bytes added to the buffer.
//!
//! This function should be used by clients who wish to add data to the buffer
//! directly rather than via calls to ringbuf_write() or ringbuf_writeone().  It
//! advances the write index by a given number of bytes.  If the
//! \e ui32NumBytes parameter is larger than the amount of free space in the
//! buffer, the read pointer will be advanced to cater for the addition.  Note
//! that this will result in some of the oldest data in the buffer being
//! discarded.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_advwrite(ringbuffer_t *psRingBuf,
                 uint32_t ui32NumBytes)
{
    uint32_t ui32Count;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Make sure we were not asked to add a silly number of bytes.
    //
    __ASSERT(ui32NumBytes <= psRingBuf->ui32Size);

    //
    // Determine how much free space we currently think the buffer has.
    //
    ui32Count = ringbuf_free(psRingBuf);

    //
    // Advance the buffer write index by the required number of bytes and
    // check that we have not run past the read index.  Note that we must do
    // this within a critical section (interrupts disabled) to prevent
    // race conditions that could corrupt one or other of the indices.
    //
#if __USE_INTERRUPT
    INT_Disable();
#endif

    //
    // Update the write pointer.
    //
    psRingBuf->ui32WriteIndex += ui32NumBytes;

    //
    // Check and correct for wrap.
    //
    if (psRingBuf->ui32WriteIndex >= psRingBuf->ui32Size) {
        psRingBuf->ui32WriteIndex -= psRingBuf->ui32Size;
    }

    //
    // Did the client add more bytes than the buffer had free space for?
    //
    if (ui32Count < ui32NumBytes) {
        //
        // Yes - we need to advance the read pointer to ahead of the write
        // pointer to discard some of the oldest data.
        //
        psRingBuf->ui32ReadIndex = psRingBuf->ui32WriteIndex + 1;

        //
        // Correct for buffer wrap if necessary.
        //
        if (psRingBuf->ui32ReadIndex >= psRingBuf->ui32Size) {
            psRingBuf->ui32ReadIndex -= psRingBuf->ui32Size;
        }
    }

    //
    // Restore interrupts if we turned them off earlier.
    //
#if __USE_INTERRUPT
    INT_Enable();
#endif
}

//*****************************************************************************
//
//! Writes a single byte of data to a ring buffer.
//!
//! \param psRingBuf points to the ring buffer to be written to.
//! \param ui8Data is the byte to be written.
//!
//! This function writes a single byte of data into a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_writeone(ringbuffer_t *psRingBuf, uint8_t ui8Data)
{
    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);

    //
    // Verify that space is available in the buffer.
    //
    __ASSERT(ringbuf_free(psRingBuf) != 0);

    //
    // Write the data byte.
    //
    psRingBuf->pui8Buf[psRingBuf->ui32WriteIndex] = ui8Data;

    //
    // Increment the write index.
    //
    update_index_atomic(&psRingBuf->ui32WriteIndex, 1, psRingBuf->ui32Size);
}

//*****************************************************************************
//
//! Writes data to a ring buffer.
//!
//! \param psRingBuf points to the ring buffer to be written to.
//! \param pui8Data points to the data to be written.
//! \param ui32Length is the number of bytes to be written.
//!
//! This function write a sequence of bytes into a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_write(ringbuffer_t *psRingBuf, uint8_t *pui8Data,
              uint32_t ui32Length)
{
    uint32_t ui32Temp;

    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);
    __ASSERT(pui8Data != NULL);
    __ASSERT(ui32Length != 0);

    //
    // Verify that space is available in the buffer.
    //
    __ASSERT(ui32Length <= ringbuf_free(psRingBuf));

    //
    // Write the data into the ring buffer.
    //
    for (ui32Temp = 0; ui32Temp < ui32Length; ui32Temp++) {
        ringbuf_writeone(psRingBuf, pui8Data[ui32Temp]);
    }
}

//*****************************************************************************
//
//! Initialize a ring buffer object.
//!
//! \param psRingBuf points to the ring buffer to be initialized.
//! \param pui8Buf points to the data buffer to be used for the ring buffer.
//! \param ui32Size is the size of the buffer in bytes.
//!
//! This function initializes a ring buffer object, preparing it to store data.
//!
//! \return None.
//
//*****************************************************************************
void
ringbuf_init(ringbuffer_t *psRingBuf, uint8_t *pui8Buf,
             uint32_t ui32Size)
{
    //
    // Check the arguments.
    //
    __ASSERT(psRingBuf != NULL);
    __ASSERT(pui8Buf != NULL);
    __ASSERT(ui32Size != 0);

    //
    // Initialize the ring buffer object.
    //
    psRingBuf->ui32Size = ui32Size;
    psRingBuf->pui8Buf = pui8Buf;
    psRingBuf->ui32WriteIndex = psRingBuf->ui32ReadIndex = 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
