#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t u32;
typedef int32_t  i32;

typedef struct {
    i32 *elements;
    u32 idx_read;
    u32 idx_write;
    
    u32 size;
    bool is_full;
} RingBuffer;

typedef RingBuffer* RingBuffer_Handle;

/**
 * @brief Initialize an RingBthe `buf` bufferuffer.
 * 
 * @param[inout] buf    A RingBuffer handle.
 * @param[in]    size   The desired buffer size.
 */
void ringbuf_init(RingBuffer_Handle buf, u32 size);

/**
 * @brief Free the memory reserved by a RingBuffer.
 * 
 * @param[in] buf A RingBuffer handle.
 */
void ringbuf_destroy(RingBuffer_Handle buf);

/**
 * @brief Check if a buffer is full.
 * 
 * @param[in] buf A RingBuffer handle.
 * @return Whether the buffer is full.
 */
bool ringbuf_isFull(RingBuffer_Handle buf);

/**
 * @brief Check if a buffer is empty.
 * 
 * @param[in] buf A RingBuffer handle.
 * @return Whether the buffer is empty.
 */
bool ringbuf_isEmpty(RingBuffer_Handle buf);

/**
 * @brief Add an element to a buffer.
 * 
 * @param[inout] buf    A RingBuffer handle.
 * @param[in]    el     An element to add.
 * @return Whether the operation was successful.
 */
bool ringbuf_push(RingBuffer_Handle buf, i32 el);

/**
 * @brief Retreive the oldest element from a buffer.
 * 
 * @param[inout] buf    A RingBuffer handle.
 * @param[out]   el     An element handle to output into.
 * @return Whether the operation was successful.
 */
bool ringbuf_pop(RingBuffer_Handle buf, i32 *el);

#ifdef __cplusplus
}
#endif

#endif /* __RING_BUFFER_H */
