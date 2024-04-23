#include "my_seq.h"

#define T Seq_T

extern T Seq_new(int HINT) {
        T new_Seq = (T)malloc(sizeof(struct T));
        new_Seq->p = (void **)malloc(sizeof(uint32_t *) * HINT);
        new_Seq->length = 0;
        new_Seq->capacity = HINT;
        return new_Seq;
}

extern uint32_t Seq_length(T Seq) {
        return Seq->length;
}

extern void * Seq_get(T Seq, uint32_t idx) {
        assert(idx < Seq->length);
        return *(Seq->p + idx);
}

/* change the return from original ith index to void return since we
   did not use this feature in um */
extern void Seq_put(T Seq, uint32_t idx, void *elem) {
        assert(idx < Seq->length);
        *(Seq->p + idx) = elem;
}

/* samely void as Seq_put */
extern void Seq_addhi(T Seq, void *elem) {
        /* enlarge capacity */
        if (Seq->length == Seq->capacity - 1) {
                if (Seq->capacity >= (1UL << 31)) {
                        Seq->capacity = -1; /* set capacity to MAX */
                }
                else {
                        Seq->capacity = Seq->capacity << 1;
                }
                void **t = (void **)malloc(sizeof(uint32_t *) * Seq->capacity);
                assert(t); /* memory exhaust */
                memcpy(t, Seq->p, Seq->length * sizeof(uint32_t *));
                free(Seq->p);
                Seq->p = t;
        }
        *(Seq->p + Seq->length) = elem;
        Seq->length++;
}

/* do return the original elem here */
extern void * Seq_remhi(T Seq) {
        /* do not delete it but just shrink the length */
        Seq->length--;
        return *(Seq->p + Seq->length);
}

extern void Seq_free(T *Seq) {
        free((*Seq)->p);
        free(*Seq);
}

#undef T