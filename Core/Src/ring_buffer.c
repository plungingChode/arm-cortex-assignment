#include "ring_buffer.h"
#include <stdlib.h>


void ringbuf_init(RingBuffer_Handle buf, u32 size) {
    buf->elements = (i32 *)malloc(size * sizeof(i32));
    buf->idx_read = 0;
    buf->idx_write = 0;
    buf->size = size;
    buf->is_full = false;
}

void ringbuf_destroy(RingBuffer_Handle buf) {
    free(buf->elements);
}

bool ringbuf_isFull(RingBuffer_Handle buf) {
    return buf->is_full;
}

bool ringbuf_isEmpty(RingBuffer_Handle buf) {
    return (buf->idx_read == buf->idx_write) && !buf->is_full;
}

bool ringbuf_push(RingBuffer_Handle buf, i32 el) {
    if (ringbuf_isFull(buf)) {
        return false;
    }
    buf->elements[buf->idx_write] = el;
    buf->idx_write = (buf->idx_write + 1) % buf->size;
    buf->is_full = buf->idx_write == buf->idx_read;
    return true;
}

bool ringbuf_pop(RingBuffer_Handle buf, i32 *el) {
    if (ringbuf_isEmpty(buf)) {
        return false;
    }
    int pop_idx = (buf->idx_read + 1) % buf->size;
    *el = buf->elements[pop_idx];
    buf->idx_read = pop_idx;
    return true;
}