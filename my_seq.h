/* This Seq ADT is only made for um. It is not polymorphic and unsafe */

#ifndef MY_SEQ_H
#define MY_SEQ_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define T Seq_T

typedef struct T {
        /* index is uint32_t */
        uint32_t length;
        uint32_t capacity;
        void **p; /* used in my um, p will always be uint32_t ** */
} * T;

extern T Seq_new(int HINT);

extern uint32_t Seq_length(T Seq);

extern void * Seq_get(T Seq, uint32_t idx);

/* change the return from original ith index to void return since we
   did not use this feature in um */
extern void Seq_put(T Seq, uint32_t idx, void *elem);

/* samely void as Seq_put */
extern void Seq_addhi(T Seq, void *elem);

/* do return the original elem here */
extern void * Seq_remhi(T Seq);

extern void Seq_free(T *Seq);

#undef T
#endif