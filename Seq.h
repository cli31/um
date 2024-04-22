#ifndef SEQ_H
#define SEQ_H

#include <stdint.h>

#define T Seq_T

typedef struct T {
    uint32_t length;
    void **p; /* used in my um, p will always be uint32_t ** */

} * T;

extern T Seq_new(int HINT) {
    T t;
    t->p = malloc(sizeof(uint32_t *) * HINT);
    length = 0;
}

#undef T
#endif